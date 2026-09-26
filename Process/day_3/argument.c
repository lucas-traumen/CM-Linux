#include <stdio.h>

int main(int argc, char *argv[]) {
    // In ra số lượng tham số
    printf("Number of arguments: %d\n\n", argc);
    
    // In ra tất cả các tham số
    printf("Arguments passed:\n");
    for (int i = 0; i < argc; i++) {
        printf("argv[%d]: %s\n", i, argv[i]);
    }
    
    return 0;
}

// ./exampSle
// ./example hello world
// ./example 123 456 789