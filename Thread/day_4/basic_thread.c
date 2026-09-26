#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void* thread_function(void* arg) {
    printf("Thread: Hello from thread!\n");
    printf("Thread: Working...\n");
    sleep(2);
    printf("Thread: Exiting\n");
    pthread_exit(NULL);
}

int main() {
    pthread_t thread;
    
    printf("Main: Creating thread\n");
    pthread_create(&thread, NULL, thread_function, NULL);
    
    printf("Main: Waiting for thread\n");
    pthread_join(thread, NULL);
    
    printf("Main: Thread finished, exiting\n");
    return 0;
}
