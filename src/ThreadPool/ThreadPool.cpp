#include "ThreadPool.h"

void ThreadPool::Start()
{
    unsigned int num_threads = std::thread::hardware_concurrency();
    threads.resize(num_threads);
    for (unsigned int i = 0; i < num_threads; i++)
    {
        threads.at(i) = std::thread(&ThreadPool::ThreadLoop, this);
    }
}

void ThreadPool::QueueJob(const std::function<void()>& job)
{
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        jobs.push(job);
    }
    mutex_condition.notify_one();
}

void ThreadPool::Stop()
{
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        should_terminate = true;
    }
    mutex_condition.notify_all();
    for (std::thread& active_thread : threads) 
    {
        active_thread.join();
    }
    threads.clear();
}

bool ThreadPool::Busy()
{
    bool poolbusy;
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        poolbusy = jobs.empty();
    }
    return poolbusy;
}

void ThreadPool::ThreadLoop()
{
    while (true) {
        std::function<void()> job;
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            mutex_condition.wait(lock, [this] {return !jobs.empty() || should_terminate;});
            if (should_terminate) 
            {
                return;
            }
            job = jobs.front();
            jobs.pop();
        }
        job();
    }
}
