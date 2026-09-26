#include <stdio.h>
#include <pthread.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
int counter_safe = 0;
int counter_unsafe = 0;

void* increment_safe(void* arg) {
    for (int i = 0; i < 100000; i++) {
        pthread_mutex_lock(&mutex);
        counter_safe++;
        pthread_mutex_unlock(&mutex);
    }
    pthread_exit(NULL);
}

void* increment_unsafe(void* arg) {
    for (int i = 0; i < 100000; i++) {
        counter_unsafe++;  // No mutex!
    }
    pthread_exit(NULL);
}

int main() {
    pthread_t t1, t2, t3, t4;
    
    printf("=== WITH Mutex (Safe) ===\n");
    pthread_create(&t1, NULL, increment_safe, NULL);
    pthread_create(&t2, NULL, increment_safe, NULL);
    pthread_join(t1, NULL);
    pthread_join(t2, NULL);
    printf("Expected: 200000\n");
    printf("Got:      %d\n\n", counter_safe);
    
    printf("=== WITHOUT Mutex (Unsafe) ===\n");
    pthread_create(&t3, NULL, increment_unsafe, NULL);
    pthread_create(&t4, NULL, increment_unsafe, NULL);
    pthread_join(t3, NULL);
    pthread_join(t4, NULL);
    printf("Expected: 200000\n");
    printf("Got:      %d (WRONG!)\n", counter_unsafe);
    
    pthread_mutex_destroy(&mutex);
    return 0;
}
