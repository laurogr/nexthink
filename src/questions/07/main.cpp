#include <iostream>
#include <map>
#include <signal.h>

//void myHandler(int signal) {
//    printf("My Handler\n");
//}

int main() {
    //signal(SIGTERM,myHandler);

    sigset_t set;
    sigemptyset(&set);
    sigaddset(&set,SIGTERM);
    sigprocmask(SIG_BLOCK,&set,nullptr);
    std::cout << "Testing SIGTERM blocking" << std::endl;
    std::string blockTest;
    std::cin >> blockTest;

    return 0;
}
