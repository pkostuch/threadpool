
#pragma once

#ifndef _THREAD_POOL_H
#define _THREAD_POOL_H

#include "TaskQueue.h"
#include <thread>
#include <vector>

namespace task {

template <typename T = std::function<void()>, unsigned Priorities = 1>
class ThreadPool
{
public:

    using TaskQueueType = TaskQueue<T, Priorities>;

    ThreadPool(unsigned size = 1)
    {

        auto procedure = [this] () {
            for(;;) {
                try {
                    /* get task and execute it */
                    auto task = _queue.get();
                    task();
                }
                catch (const typename TaskQueueType::Exception& e) {
                    /* queue is close, terminate worker */
                    break;
                }
                catch (...) {
                    /* any other exception, ignore */
                }
            }
        };

        for (auto i = 0u; i < size; ++i) {
            _workers.emplace_back(procedure);
        }
    }

    ~ThreadPool()
    {
        _queue.close();
        for(auto &worker : _workers) {
            if (worker.joinable())
                worker.join();
        }
    }

    template <typename Task>
    void push(Task &&task, unsigned prio = 0) {
        _queue.push(std::forward<Task>(task));
    }

private:

    TaskQueueType _queue;

    std::vector<std::thread> _workers;
};

} // namespace task

#endif
