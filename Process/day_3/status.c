#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
    int status = 9999;  // Initialize with garbage
    
    printf("Before fork: status = %d\n", status);
    
    pid_t pid = fork();
    
    if (pid == 0) {
        // Child process
        printf("Child: Exiting with code 42\n");
        exit(42);  // ← Step 1: Child calls exit(42)
    }
    
    // Parent process
    printf("Before wait: status = %d (still garbage)\n", status);
    
    // Step 2: Parent calls wait()
    // Step 3: Kernel updates status
    wait(&status);  // ← Kernel writes exit info here
    
    printf("After wait: status = %d (updated by kernel)\n", status);
    
    // Step 4: Extract exit code
    if (WIFEXITED(status)) {
        int exit_code = WEXITSTATUS(status);
        printf("Exit code: %d\n", exit_code);
    }
    
    return 0;
}
