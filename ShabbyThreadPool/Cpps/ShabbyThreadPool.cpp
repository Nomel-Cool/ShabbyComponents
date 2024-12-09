#include "ShabbyThreadPool.h"

ShabbyThreadPool& ShabbyThreadPool::GetInstance()
{
    static ShabbyThreadPool instance(std::thread::hardware_concurrency()); // 使用硬件并发线程数
    return instance;
}

ShabbyThreadPool::ShabbyThreadPool(size_t numThreads) : stop(false) 
{
    for (size_t i = 0; i < numThreads; ++i)
        workers.emplace_back([this] { this->worker(); });
}

ShabbyThreadPool::~ShabbyThreadPool()
{
    stop.store(true);
    task_queue.AddQuestToQueue(nullptr); // 确保所有等待中的线程退出
    for (std::thread& worker : workers)
        if (worker.joinable())
            worker.join();
}

// 工作线程函数
void ShabbyThreadPool::worker() 
{
    while (!stop.load())
    {
        auto task = std::move(task_queue.GetQuestFromQueue());
        if (task)
            (*task)();
    }
}
