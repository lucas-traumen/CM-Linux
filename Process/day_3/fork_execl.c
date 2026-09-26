#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t pid;
    
    printf("Parent PID: %d\n", getpid());
    
    pid = fork();
    
    if (pid < 0) {
        perror("Fork failed");
        return 1;
    }
    else if (pid == 0) {
        // Child process
        printf("Child: Executing 'ls -l'\n");
        execl("/bin/ls", "ls", "-l", NULL);
        
        // If execl successes then it won't be called
        perror("execl failed");
        return 1;
    }
    else {
        // Parent process
        printf("Parent: Waiting for child %d\n", pid);
        wait(NULL);
        printf("Parent: Child finished!\n");
    }
    
    return 0;
}
