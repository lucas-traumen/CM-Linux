#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>


pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

int flag_data =0;
int data=0;

void* Producer(void* arg)
{
    srandom(time(NULL));


    for(int i =0 ;i<10;i++)
    {

        sleep(1);
        printf("Producer: I'm ready\n");
        pthread_mutex_lock(&mutex);

        data = random()%100;
        flag_data=1;
        pthread_cond_signal(&cond);
        pthread_mutex_unlock(&mutex);

        sleep(1);

        flag_data=0;
    }

    pthread_exit(NULL);

}
void* Consumer(void* arg)
{
    for(int i =0 ;i<10;i++)
    {
    printf("Consumer: I'm ready\n");

    pthread_mutex_lock(&mutex);
    while(flag_data==0)
    {
        pthread_cond_wait(&cond,&mutex); // Here wait release key mutex and provider claim it
    }
    printf("Consumer %d: Data = %d\n",i,data);
    pthread_mutex_unlock(&mutex);
    sleep(1);
}
    pthread_exit(NULL);
}

int main()
{

    pthread_t thread1;
    pthread_t thread2;


    printf("Main: Creating Producer\n");
    pthread_create(&thread1, NULL, Producer,NULL);
    printf("Main: Creating Consumer\n");
    pthread_create(&thread2, NULL, Consumer,NULL);

    pthread_detach(thread1);
    pthread_detach(thread2);

    while(1);


    return 0;
}
