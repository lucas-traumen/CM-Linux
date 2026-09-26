#include <stdio.h>
#include <unistd.h>    // getpid()
#include <errno.h>    // perror()
#include <sys/wait.h> // wait()
#include <stdlib.h> //exit()

int main(void)
{
    pid_t pid1;
    int status;
    printf ("Parent PID : %d\n\n",getpid());
// pid1 in child process it value =0 else parent process pid1 = child_pid
    pid1 =fork();
    if(pid1 <0)
    {
        perror("Wrong create new thread");
    }
    else if( pid1 == 0)
    {
        printf("Child  (PID %d): Starting...\n", getpid());
        printf("Parent in child process (PID %d):    \n", getppid());
        printf("pid in child : %d\n", pid1);
        sleep(1);
        printf("Child : Exiting with code 10\n");
        exit(10);
    }else {
        printf("Child  (PID %d): Starting...\n", getpid());
        printf("pid in parent : %d\n", pid1);
        wait(&status);
        if(WIFEXITED(status))
        printf("Parent: Child finished with code %d-%x\n",
               WEXITSTATUS(status),WEXITSTATUS(status));


    }
    return 0;
}
