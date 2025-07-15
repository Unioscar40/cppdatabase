## Basic threads

```cpp
#include <iostream>
#include <thread>

void 
worker(int id,const std::string &message)
{
    std::cout << "worker id: " << id  << message << std::endl;
}

int main()
{
    std::thread thread1(worker, 1, "hello");
    std::thread thread2(worker, 2, "world");

    thread1.join();
    thread2.join();

    std::cout << "All threads are finished" << std::endl;

    return 0;

}

```