#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

pthread_mutex_t mutex;
int a=0;
void* thread_function1(void* arg) {

    printf("Thread1: Hello from thread! my tid:%ld \n",pthread_self());
    printf("get pid: %d\n",getpid());
   // pthread_mutex_lock(&mutex);
    printf("Thread1: Working...\n");
    for( unsigned int i=0;i<1000000000;i++) ++a;
  //  pthread_mutex_unlock(&mutex);
    printf("Thread1: Exiting with a=%d\n",a);
    pthread_exit("HI");
}
void* thread_function2(void* arg) {
    printf("Thread2: Hello from thread! my tid:%ld \n",pthread_self());
    printf("get pid: %d\n",getpid());
    //printf("The argument value: %d \n ",*(int*)arg);
    printf("Thread2: Working...\n");
   // pthread_mutex_lock(&mutex);
    for( unsigned int i=0;i<1000000000;i++) ++a;
  //  pthread_mutex_unlock(&mutex);
    printf("Thread1: Exiting with a=%d\n",a);
    pthread_exit("HELLO");
}

int main(void)
{

    pthread_t thread1;
    pthread_t thread2;

    pthread_mutex_init(&mutex,NULL);

    printf("Main: Creating thread1\n");
    pthread_create(&thread1, NULL, thread_function1,NULL);
    printf("Main: Creating thread2\n");
    pthread_create(&thread2, NULL, thread_function2,NULL);

    pthread_detach(thread1);
    pthread_detach(thread2);

    sleep(20);

    pthread_mutex_destroy(&mutex);

    return 0;
}
