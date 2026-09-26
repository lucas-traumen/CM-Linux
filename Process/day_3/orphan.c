#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t pid = fork();
    
    if (pid == 0) {
        // Child process
        printf("Child: PID=%d, Parent PID=%d\n", getpid(), getppid());
        sleep(5);  // Wait for parent to die
        printf("Child: PID=%d, Parent PID=%d (adopted by init/systemd)\n", 
               getpid(), getppid());
        sleep(5);
        printf("Child: Exiting\n");
    }
    else {
        // Parent process
        printf("Parent: PID=%d, Child PID=%d\n", getpid(), pid);
        sleep(2);
        printf("Parent: Exiting \n");
    }
    
    return 0;
}
