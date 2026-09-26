#include <stdio.h>
#include <pthread.h>

void* print_number(void* arg) {
    int num = *(int*)arg;
    printf("Thread: My number is %d\n", num);
    pthread_exit(NULL);
}

int main() {
    pthread_t thread;
    int number = 42;
    
    printf("Main: Creating thread with number %d\n", number);
    pthread_create(&thread, NULL, print_number, &number);
    
    pthread_join(thread, NULL);
    printf("Main: Done\n");
    
    return 0;
}
