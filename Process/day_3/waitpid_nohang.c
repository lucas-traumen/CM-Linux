#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    pid_t pid, result;
    int status;
    
    pid = fork();
    
    if (pid == 0) {
        // Child - sleep 5 seconds
        printf("Child (PID %d): Sleeping for 5 seconds...\n", getpid());
        sleep(5);
        printf("Child: Done sleeping, exiting!\n");
        exit(42);
    }
    else {
        // Parent - check child every second
        printf("Parent: Child PID is %d\n", pid);
        
        int count = 0;
        while (1) {
            // WNOHANG = non-blocking, return immediately
            result = waitpid(pid, &status, WNOHANG);
            
            if (result == 0) {
                // Child still running
                printf("Parent: Child still running... (%d sec)\n", ++count);
                sleep(1);
            }
            else if (result == pid) {
                // Child finished
                printf("Parent: Child finished with code %d\n", 
                       WEXITSTATUS(status));
                break;
            }
            else {
                perror("waitpid error");
                break;
            }
        }
    }
    
    return 0;
}
