
#pragma once

#ifndef _TASK_QUEUE_POOL_H
#define _TASK_QUEUE_POOL_H

#include "ThreadPool.h"
#include <vector>

namespace task {

template <typename T = std::function<void()>, unsigned Priorites = 1>
class TaskQueuePool
{
public:

    /* Create pool of thread pools, each thread pool has one dedicated queue and one worker. */
    TaskQueuePool(unsigned size) {
        for (auto i = 0u; i < size; ++i) {
            _pools.emplace_back(1);
        }
    }

    template <typename Task>
    void push(unsigned queue, Task &&task, unsigned prio = 0) {
        _pools[queue].push(std::forward<Task>(task), prio);
    }

    decltype(auto) size() const { return _pools.size(); }

private:

    std::deque<ThreadPool<T, Priorites>> _pools;
};

}

#endif
