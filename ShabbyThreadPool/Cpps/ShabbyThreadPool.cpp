#include "ShabbyThreadPool.h"

ShabbyThreadPool& ShabbyThreadPool::GetInstance()
{
    static ShabbyThreadPool instance(std::thread::hardware_concurrency()); // ʹ��Ӳ�������߳���
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
    task_queue.AddQuestToQueue(nullptr); // ȷ�����еȴ��е��߳��˳�
    for (std::thread& worker : workers)
        if (worker.joinable())
            worker.join();
}

// �����̺߳���
void ShabbyThreadPool::worker() 
{
    while (!stop.load())
    {
        auto task = std::move(task_queue.GetQuestFromQueue());
        if (task)
            (*task)();
    }
}
