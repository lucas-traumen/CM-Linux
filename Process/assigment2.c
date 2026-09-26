#include <stdio.h>
#include <unistd.h>    // getpid() execl()
#include <errno.h>    // perror()
#include <sys/wait.h> // wait()
#include <stdlib.h> //exit()
#include <string.h>

int main ( int argc, char *argv[])
{
    pid_t pid;
    int status;
    if ( argc<0)
    {
        perror("Wrong\n");
        return 1;
    }else if ( argc ==1)
    {
        perror(" Please entering command variables");
        return 1;
    }

    setenv("MY_COMMAND", "ls", 1);
    pid = fork();
    if (pid < 0) {
        perror("Fork failed");
        return 1;
    }
    else if (pid == 0) {
        // Child process
        sleep(10);
        char *cmd = getenv("MY_COMMAND");
        execlp(cmd,cmd,"-l",(char*)NULL);// first it take ls and look up "$PATH"

        // If execlp successes then it won't be called
        perror("execlp failed");
        return 1;
    }
    else {
        //setenv("MY_COMMAND", "ls", 1);
        // Parent process
        printf("Parent: Waiting for child %d\n", pid);
        waitpid(pid,&status,0);
        printf("Parent: Child finished!\n");
    }
    return 0;
}
