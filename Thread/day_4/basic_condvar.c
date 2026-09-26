// comparison.c
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
int ready = 0;
int test_mode = 0;  // 0 = signal, 1 = broadcast

void* waiter(void* arg) {
    int id = *(int*)arg;
    
    pthread_mutex_lock(&mutex);
    
    while (ready == 0) {
        pthread_cond_wait(&cond, &mutex);
    }
    
    printf("  Thread %d: Woke up!\n", id);
    pthread_mutex_unlock(&mutex);
    
    pthread_exit(NULL);
}

void test_signal() {
    pthread_t threads[3];
    int ids[3] = {1, 2, 3};
    ready = 0;
    
    printf("\n=== TEST 1: pthread_cond_signal() ===\n");
    printf("Wakes ONE thread at a time\n\n");
    
    for (int i = 0; i < 3; i++) {
        pthread_create(&threads[i], NULL, waiter, &ids[i]);
    }
    
    sleep(1);
    
    for (int i = 1; i <= 3; i++) {
        sleep(1);
        pthread_mutex_lock(&mutex);
        ready = 1;
        printf("Signal #%d: Wake one thread\n", i);
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);
        usleep(100000);
        ready = 0;
    }
    
    for (int i = 0; i < 3; i++) {
        pthread_join(threads[i], NULL);
    }
}

void test_broadcast() {
    pthread_t threads[3];
    int ids[3] = {4, 5, 6};
    ready = 0;
    
    printf("\n=== TEST 2: pthread_cond_broadcast() ===\n");
    printf("Wakes ALL threads at once\n\n");
    
    for (int i = 0; i < 3; i++) {
        pthread_create(&threads[i], NULL, waiter, &ids[i]);
    }
    
    sleep(1);
    
    pthread_mutex_lock(&mutex);
    ready = 1;
    printf("Broadcast: Wake ALL threads\n");
    pthread_cond_broadcast(&cond);
    pthread_mutex_unlock(&mutex);
    
    for (int i = 0; i < 3; i++) {
        pthread_join(threads[i], NULL);
    }
}

int main() {
    test_signal();
    test_broadcast();
    
    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&cond);
    
    return 0;
}
