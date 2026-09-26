#include <stdio.h>
#include <pthread.h>
#include <unistd.h>



void* thread_function1(void* arg) {

    printf("Thread1: Hello from thread! my tid:%ld \n",pthread_self());
    printf("get pid: %d\n",getpid());
    printf("The argument value: %d \n ",*(int*)arg);
    printf("Thread1: Working...\n");
    sleep(1);
    printf("Thread1: Exiting\n");
    pthread_exit("HI");
}
void* thread_function2(void* arg) {
    printf("Thread2: Hello from thread! my tid:%ld \n",pthread_self());
    printf("get pid: %d\n",getpid());
    //printf("The argument value: %d \n ",*(int*)arg);
    printf("Thread2: Working...\n");
    sleep(2);
    printf("Thread2: Exiting\n");
    pthread_exit("HELLO");
}

int main()
{
    pthread_t thread1;
    pthread_t thread2;

    int value1=100;

    void* status1;
    void* status2;


    printf("Main: Creating thread1\n");
    pthread_create(&thread1, NULL, thread_function1,&value1 );
    printf("Main: Creating thread2\n");
    pthread_create(&thread2, NULL, thread_function2, NULL);

    pthread_join(thread1,&status1);
    pthread_join(thread2,&status2);

    pthread_detach(thread1);
    pthread_detach(thread2);

    //%p used to print or read a memory address (pointer value).
    printf("Thread1 status : %d \n",*(int*)status1);
    printf("Thread2 status : %d \n",*(int*)status2);


    return 0;
}
