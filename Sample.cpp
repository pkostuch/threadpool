
#include "ThreadPool.h"
#include <thread>
#include <atomic>
#include <iostream>

int main(int argc, char *argv[])
{
    std::atomic<unsigned> counter{0};    
    {
        task::ThreadPool<> threadPool{4};
        
        auto task = [&counter] () {
            counter++;
        };
        
        auto number{100};

        for (auto n{0}; n < number; ++n)
            threadPool.push(task);
    }
    
    std::cout << "counter = " << counter << std::endl;
    
    return 0;
}

