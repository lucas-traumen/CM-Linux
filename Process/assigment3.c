#include <stdio.h>
#include <unistd.h>    // getpid() execl()
#include <errno.h>    // perror()
#include <sys/wait.h> // wait()
#include <stdlib.h> //exit()
#include <string.h>
#include <signal.h> // signal()

void foo(int signum)
{
    printf("in foo\n");
    if (signum==SIGCHLD)
      //  waitpid(-1,NULL,WNOHANG);
    printf("out foo\n");
}

int main(void)
{
    pid_t pid;
    //int status;

    pid=fork();
    if (pid < 0) {
        perror("Fork failed");
        return 1;
    }
    else if (pid == 0) {
        // Child process

        // printf("pid in child : %d\n", getpid());
        // orphan when parrent die before child running
        printf("Child: PID=%d, PPID=%d\n", getpid(), getppid());

        sleep(5); // Trong lúc này cha sẽ kết thúc

        printf("Child after 5s: PID=%d, PPID=%d\n",
               getpid(), getppid());
        exit(0);
    }
    else{
        // Parent process
        //zombie parent not wait()
         printf("Parent: Waiting for child %d\n", pid);
         signal(SIGCHLD,foo);
        // //waitpid(pid,&status,0);

         printf("Parent: Child finished!\n");
         while(1);
    }

    return 0;
}
