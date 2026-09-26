#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    pid_t pid = fork();
    
    if (pid == 0) {
        // Child process
        printf("Child: PID=%d, Parent PID=%d\n", getpid(), getppid());
        printf("Child: Exiting now\n");
        exit(0);  // Child exits immediately
    }
    else {
        // Parent process
        printf("Parent: PID=%d, Child PID=%d\n", getpid(), pid);
        printf("Parent: NOT calling wait() - child becomes zombie\n");
        printf("Parent: Sleeping for 30 seconds...\n");
        printf("Parent: Check with: ps aux | grep %d\n", pid);
        sleep(30);  // Parent sleeps WITHOUT calling wait()
        printf("Parent: Exiting\n");
        // Zombie will be cleaned up when parent exits
    }
    
    return 0;
}
