#include <iostream>
#include "hello.h"

int main(void) {
    hello();
    std::string name1;
    if (valid_name(name1)) {
        std::cout<<"Hello, "<<name1<<std::endl;
    } else {
        std::cout<<"Invalid name entered."<<std::endl;
    }
    std::string name2("hello world");
    if (valid_name(name2)) {
        std::cout<<"Hello, "<<name2<<std::endl;
    } else {
        std::cout<<"Invalid name entered."<<std::endl;
    }
    return 0;
}
