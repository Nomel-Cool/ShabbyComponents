#pragma once
#ifndef SHABBY_THREAD_POOL_H
#define SHABBY_THREAD_POOL_H

#include <thread>
#include <algorithm>
#include <functional>
#include <memory>
#include <vector>
#include <mutex>
#include <atomic>
#include <future>

#include "../ShabbyUniversalType/QuestType.h"
#include "../ShabbyEventsQueue/EventsQueue.h"

namespace shabby
{
    /// <summary>
    /// 继承QuestType的用于线程池调用的事件节点类，用于记录节点保存到线程池
    /// </summary>
    class QuestNode : public QuestType
    {
    public:
        QuestNode(const QuestType& quest) : QuestType(quest), isUsing(false), isTerminated(false) {}
        bool isUsing;
        bool isTerminated;
        std::string quest_id;
    };

    /// <summary>
    /// 继承QuestType的用于线程池使用的线程节点，用于保存线程的状态
    /// </summary>
    class ThreadNode
    {
    public:
        ThreadNode() {}
        std::thread thread_node;
    };

    /// <summary>
    /// 一个简陋的线程池：
    /// 
    /// 锁设计:
    /// 线程容器【等待】: 当任务列表为空时
    /// 线程容器【苏醒】: 任务列表插入任务节点时
    /// 
    /// 组成：
    /// 一个有限线程元容器
    /// 一个无限的任务队列
    /// 操作：
    /// 任务以队列方式与线程元关联后被消费
    /// 线程元被任务消费过程中需要加锁，同时任务完成前也需要加锁
    /// 支持任务滞后操作（当任务没有在规定条件下被执行，将被滞后或取消）
    /// 
    /// 原理：
    /// 任务队列是线程安全的，消费者是线程池，生产者是任务节点
    /// 任务节点添加到线程池唤醒线程池
    /// 线程池中的【每一个线程都处于争抢与监听任务队列】条件循环状态
    /// 
    /// 数据流转：
    /// 1.加入任务节点
    /// 2.线程池某一线程争抢得到任务节点并【阻塞它自己】执行
    /// 3.线程执行完所获取的任务后，继续投入监听任务队列
    /// 
    /// </summary>
    class ShabbyThreadPool
    {
    public:
        static ShabbyThreadPool* GetPoolInstance()
        {
            /*
             * std::memory_order_acquire这个内存顺序标记。
             * 这个标记的含义是：在这个加载操作之后的所有读操作和写操作都不能被重新排序到这个加载操作之前12。
             * 换句话说，这个加载操作之后的所有操作（在当前线程中）都会看到这个加载操作的结果。
            */
            ShabbyThreadPool* tmp = instance.load(std::memory_order_acquire);
            if (!tmp) {
                std::lock_guard<std::mutex> lock(init_mutex);
                tmp = instance.load(std::memory_order_relaxed);
                if (!tmp) {
                    tmp = new ShabbyThreadPool;
                    instance.store(tmp, std::memory_order_release);
                }
            }
            return tmp;
        }

        /// <summary>
        /// 重构线程池容量
        /// </summary>
        /// <param name="thread_amount">线程池内线程数量</param>
        /// <returns>容量更改操作结果</returns>
        void ResizePool(int thread_amount)
        {
            ThreadList.resize(thread_amount);
            BusyWatcher = (int)pow(2, thread_amount) - 1;
        }

        /// <summary>
        /// 将一个任务节点初始化它的特征后放入池中，然后获取线程执行结果凭证
        /// </summary>
        /// <param name="quest">要求异步函子</param>
        template<typename RETURN_TYPE, typename... ARGS_TYPE>
        void AddQuestToPool(std::string quest_id, QuestType async_quest)
        {
            std::unique_lock lock(quest_mutex);
            QuestNode p_node(async_quest);
            p_node.quest_id = quest_id;
            QuestList.AddQuestToQueue(p_node);
            _cond.notify_one(); // 当任务队列加入任务时唤醒线程容器进行消费
        }

        /// <summary>
        /// 从任务队列中消费一个任务
        /// </summary>
        /// <typeparam name="RETURN_TYPE">函子返回值，由于是回调应当为void</typeparam>
        /// <typeparam name="...ARGS_TYPE">函子调用参数类型列表</typeparam>
        /// <return>返回任务指针</return>
        std::shared_ptr<std::packaged_task<QuestType()>> ConsumeQuestFromPool()
        {
            std::unique_lock lock(quest_mutex);
            while (QuestList.Empty()) // 当可用线程为空时 或者 任务列表为空时 将无法消费遂执行阻塞
                _cond.wait(lock); // 在等待时会自动释放锁
            auto quest_node = QuestList.GetQuestFromQueue();
            auto async_func = quest_node->GetAsyncFunctor(quest_node->quest_id);
            auto func = quest_node->CastAnyToPromise<QuestType>(async_func);
            if (func == nullptr)
            {
                std::cout << "Empty func" << std::endl;
                return nullptr;
            }
            return func;
        }

        /// <summary>
        /// 
        /// </summary>
        /// <param name="thread_index"></param>
        void work_thread(int thread_index)
        {
            while (!stop_flag)
            {
                auto p_func = ConsumeQuestFromPool();
                UpdateBusyWatcher(thread_index); // 任务开始时
                (*p_func)();
                UpdateBusyWatcher(thread_index); // 任务完成后
            }
        }

        /// <summary>
        /// 之前一直理解错了，线程池的每个线程生来都有自己的任务的就是不停地获取任务队列的任务
        /// </summary>
        void LaunchPool()
        {
            auto it_thread_list = ThreadList.begin();
            for (; it_thread_list != ThreadList.end(); ++it_thread_list)
            {
                it_thread_list->thread_node = std::thread(&ShabbyThreadPool::work_thread, this, it_thread_list - ThreadList.begin());
            }
        }

        /// <summary>
        /// 释放线程池所有线程包括自身
        /// </summary>
        void StopPool()
        {
            stop_flag = true;
            for (auto& t : ThreadList)
            {
                if (t.thread_node.joinable())
                    t.thread_node.join();
            }
            if (worker_thread.joinable())
            {
                worker_thread.join();
            }
        }

    private:
        EventsQueue<QuestNode> QuestList;
        std::vector<ThreadNode> ThreadList;
        int BusyWatcher;

        std::atomic<bool> stop_flag = false;
        std::thread worker_thread;

        std::condition_variable _cond;
        std::mutex quest_mutex;

        std::mutex watcher_mutex;

        static std::atomic<ShabbyThreadPool*> instance;
        static std::mutex init_mutex;

    private:
        ShabbyThreadPool() { /*...*/ }
        ~ShabbyThreadPool() { /*...*/ }
        ShabbyThreadPool(const ShabbyThreadPool&) = delete;
        ShabbyThreadPool& operator=(const ShabbyThreadPool&) = delete;

        size_t ConvertBusyWatcherToIndex()
        {
            unsigned long index;
            if (_BitScanForward(&index, BusyWatcher)) // 微软版本，在GCC和Clang中，你可以使用__builtin_ffs函数来达到同样的效果
            {
                BusyWatcher &= ~(1 << index); // 将找到的位设置为0
                return index;
            }
            return -1; // 如果所有的位都是0，返回-1表示没有可用的线程
        }

        /// <summary>
        /// 更新当前线程空闲状态
        /// </summary>
        /// <param name="using_index">被使用线程index</param>
        void UpdateBusyWatcher(int using_index)
        {
            std::lock_guard lock(watcher_mutex);
            BusyWatcher ^= (1 << using_index); // 将第using_index位异或1，其余异或0
        }
    };

    // 在类外初始化静态成员
    std::atomic<ShabbyThreadPool*> ShabbyThreadPool::instance;
    std::mutex ShabbyThreadPool::init_mutex;
}
#endif // !SHABBY_THREAD_POOL_H
