#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

void* joined_thread(void* arg) {
    printf("Joined thread: Working...\n");
    sleep(2);
    printf("Joined thread: Done\n");
    pthread_exit(NULL);
}

void* detached_thread(void* arg) {
    printf("Detached thread: Working...\n");
    sleep(2);
    printf("Detached thread: Done\n");
    pthread_exit(NULL);
}

int main() {
    pthread_t t1, t2;
    
    printf("=== With Join ===\n");
    pthread_create(&t1, NULL, joined_thread, NULL);
    printf("Main: Waiting for joined thread...\n");
    pthread_join(t1, NULL);  // Must wait
    printf("Main: Joined thread finished\n\n");
    
    printf("=== With Detach ===\n");
    pthread_create(&t2, NULL, detached_thread, NULL);
    pthread_detach(t2);  // Don't wait
    printf("Main: Detached thread, continuing immediately\n");
    
    // sleep(3);  // Give detached thread time to finish
    printf("Main: Exiting\n");
    
    return 0;
}
