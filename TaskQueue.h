#pragma once

#ifndef _TASK_QUEUE_H
#define _TASK_QUEUE_H

#include <deque>
#include <mutex>
#include <condition_variable>
#include <array>
#include <stdexcept>

namespace task {

/*
 * Task queue with priorities. You can control number of priorities through template argument.
 * The priorities are 0, 1, 2... N, where 0 is the highest one.
 */
template<typename T, unsigned Priorities>
class TaskQueue
{
public:

    class Exception : std::runtime_error
    {
    public:
        Exception() : std::runtime_error("TaskQueue is no longer active") {}
    };

    TaskQueue() = default;

    TaskQueue(const TaskQueue&) = delete;

    TaskQueue& operator=(const TaskQueue&) = delete;

    ~TaskQueue() {
        close();
    }

    /*
     * Push task into a queue.
     * You can specify task's priority (0 means highest).
     */
    template<typename Task>
    void push(Task &&task, unsigned prio = 0) {
        std::unique_lock<std::mutex> lock(_mutex);
        _queues[prio].push_back(std::forward<Task>(task));
        _condition.notify_one();
    }

    /*
     * Get task from queue. This is blocking operation.
     * If queue is closed, then in case there're no more tasks there's an exception thrown.
     */
    T get() {
        std::unique_lock<std::mutex> lock(_mutex);
        while (_empty()) {
            if (!_active)
                throw Exception{};
            _condition.wait(lock);
        }
        return _get();
    }

    /*
     *
     */
    void close() {
        std::unique_lock<std::mutex> lock(_mutex);
        _active = false;
        _condition.notify_all();
    }

private:

    bool _empty() {
        for(auto &q : _queues) {
            if (!q.empty())
                return false;
        }
        return true;
    }

    /*
     * Retrieve task from queue.
     */
    T _get() {
        for(auto i{0u}; i < _queues.size(); ++i) {
            auto &queue = _queues[i];
            if (queue.empty())
                continue;
            T t{std::move(queue.front())};
            queue.pop_front();
            return t;
        }
        // this should not happen
        throw std::runtime_error("");
    }

    bool _active{true};

    std::mutex _mutex;

    std::condition_variable _condition;

    using Queue = std::deque<T>;

    /*
     * This is an array of queues. Each queue in array represents different priorities.
     */
    std::array<Queue, Priorities> _queues;

};

} // namespace task

#endif
