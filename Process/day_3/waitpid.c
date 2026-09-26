#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t pid1, pid2, pid3;
    int status;
    
    printf("Parent PID: %d\n\n", getpid());
    
    // Create child 1
    pid1 = fork();
    if (pid1 == 0) {
        printf("Child 1 (PID %d): Starting...\n", getpid());
        sleep(1);
        printf("Child 1: Exiting with code 10\n");
        exit(10);
    }
    
    // Create child 2
    pid2 = fork();
    if (pid2 == 0) {
        printf("Child 2 (PID %d): Starting...\n", getpid());
        sleep(2);
        printf("Child 2: Exiting with code 20\n");
        exit(20);
    }
    
    // Create child 3
    pid3 = fork();
    if (pid3 == 0) {
        printf("Child 3 (PID %d): Starting...\n", getpid());
        sleep(3);
        printf("Child 3: Exiting with code 30\n");
        exit(30);
    }
    
    // Parent waits for each child
    printf("\nParent: Waiting for child 1 (PID %d)...\n", pid1);
    waitpid(pid1, &status, 0);
    printf("Parent: Child 1 exited with code %d\n\n", WEXITSTATUS(status));
    
    printf("Parent: Waiting for child 2 (PID %d)...\n", pid2);
    waitpid(pid2, &status, 0);
    printf("Parent: Child 2 exited with code %d\n\n", WEXITSTATUS(status));
    
    printf("Parent: Waiting for child 3 (PID %d)...\n", pid3);
    waitpid(pid3, &status, 0);
    printf("Parent: Child 3 exited with code %d\n\n", WEXITSTATUS(status));
    
    printf("Parent: All children finished!\n");
    return 0;
}
