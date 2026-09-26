#include <stdio.h>
#include <pthread.h>

void* thread_function(void* arg) {
    pthread_t my_id = pthread_self();
    pthread_t* main_id = (pthread_t*)arg;
    
    if (pthread_equal(my_id, *main_id)) {
        printf("Thread: I am the SAME as main thread\n");
    } else {
        printf("Thread: I am DIFFERENT from main thread\n");
    }
    
    pthread_exit(NULL);
}

int main() {
    pthread_t thread;
    pthread_t main_id = pthread_self();
    
    pthread_create(&thread, NULL, thread_function, &main_id);
    pthread_join(thread, NULL);
    
    return 0;
}
