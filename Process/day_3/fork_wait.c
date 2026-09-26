#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t pid;
    
    printf("Parent process PID: %d\n", getpid());
    
    pid = fork();
    
    if (pid < 0) {
        // Fork failed
        perror("Fork failed");
        return 1;
    }
    else if (pid == 0) {
        // Child process
        printf("Child process PID: %d, Parent PID: %d\n", 
               getpid(), getppid());
        printf("Child: I'm doing some work...\n");
        sleep(2);
        printf("Child: Work done!\n");
    }
    else {
        // Parent process
        printf("Parent: Created child with PID %d\n", pid);
        printf("Parent: Waiting for child...\n");
        
        wait(NULL);  // Wait child to finish
        
        printf("Parent: Child has finished!\n");
    }
    
    return 0;
}
