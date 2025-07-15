#include <iostream>
#include <thread>

int main()
{
    std::thread t;

    t = std::thread([] (){
        std::cout << "Work 1 finish" << std::endl;
    });

    t.join();

    t = std::thread([]() {
        std::cout << "Work 2 finish" << std::endl;
    });

    t.join();

    t = std::thread([]() {
        std::cout << "Work finished" << std::endl;
    });

    t.join();

    return 0;

}