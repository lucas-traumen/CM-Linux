#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void* calculate_square(void* arg) {
    int num = *(int*)arg;
    int* result = malloc(sizeof(int));
    
    *result = num * num;
    
    printf("Thread: %d * %d = %d\n", num, num, *result);
    pthread_exit(result);  // Return result
}

int main() {
    pthread_t thread;
    int number = 7;
    int* result;
    
    pthread_create(&thread, NULL, calculate_square, &number);
    pthread_join(thread, (void**)&result);
    
    printf("Main: Result from thread = %d\n", *result);
    
    free(result);
    return 0;
}
