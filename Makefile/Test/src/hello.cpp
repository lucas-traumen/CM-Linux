#include "hello.h"

void hello() {
    std::cout<<"Hello, World"<<std::endl;

}
bool valid_name(std::string name) {
    std::cout<<"Checking your name "<<name<<"................."<<std::endl;
    //std::cin>>name;
    if (name.empty()) {
        return false;
    }
    return true;
}
