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

/// <summary>
/// 继承QuestType的用于线程池调用的事件节点类，用于记录节点保存到线程池
/// </summary>
class QuestNode : public QuestType
{
public:
	QuestNode():isUsing(false),isTerminated(false){}
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
    bool isBusy;
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
            std::lock_guard<std::mutex> lock(mutex);
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
    bool ResizePool(int thread_amount)
    {
        ThreadList.resize(thread_amount);
        BusyWatcher = (int)pow(2, thread_amount) - 1;
    }

    /// <summary>
    /// 将一个任务节点初始化它的特征后放入池中，然后获取线程执行结果凭证
    /// </summary>
    /// <param name="quest">要求异步函子</param>
    template<typename RETURN_TYPE, typename... ARGS_TYPE>
    void AddQuestToPool(QuestType async_quest, std::string quest_id)
    {
        std::unique_lock lock(_mutex);
        auto any_func = async_quest.GetFunctor("quest_id");
        auto concrete_func = async_quest.CastAnyToPromise<RETURN_TYPE>(any_func);
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
    /// <param name="func_id">函子ID</param>
    /// <param name="...args">实参列表</param>
    void ConsumeQuestFromPool(std::string func_id)
    {
        std::unique_lock lock(_mutex);
        while (ConvertBusyWatcherToIndex() == -1 || QuestList.Empty()) // 当可用线程为空时 或者 任务列表为空时 将无法消费遂执行阻塞
            _cond.wait(lock); // 在等待时会自动释放锁
        size_t available_thread_index = ConvertBusyWatcherToIndex();
        auto quest_node = QuestList.GetQuestFromQueue();
        auto async_func = quest_node->GetAsyncFunctor(quest_node->quest_id);
        auto func = quest_node->CastAnyToPromise<QuestType>(async_func);
        (*func)();
    }

    /// <summary>
    /// 线程池运行，执行消费
    /// </summary>
    void LaunchPool()
    {
        while (true)
        {

        }
    }

private:
    EventsQueue<QuestNode> QuestList;
    std::vector<ThreadNode> ThreadList;
    int BusyWatcher;

    std::condition_variable _cond;
    std::mutex _mutex;

    ShabbyThreadPool() { /*...*/ }
    ~ShabbyThreadPool() { /*...*/ }
    ShabbyThreadPool(const ShabbyThreadPool&) = delete;
    ShabbyThreadPool& operator=(const ShabbyThreadPool&) = delete;

    size_t ConvertBusyWatcherToIndex()
    {

    }

    static std::atomic<ShabbyThreadPool*> instance;
    static std::mutex mutex;
};

// 在类外初始化静态成员
std::atomic<ShabbyThreadPool*> ShabbyThreadPool::instance;
std::mutex ShabbyThreadPool::mutex;

#endif // !SHABBY_THREAD_POOL_H
