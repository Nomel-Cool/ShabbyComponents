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

#include "../ShabbyUniversalType/QuestType.h"
#include "../ShabbyEventsQueue/EventsQueue.h"

/// <summary>
/// 继承QuestType的用于线程池调用的事件节点类，用于记录节点保存到线程池
/// </summary>
class QuestNode : public QuestType
{
public:
	QuestNode():isUsing(false),isTerminated(false),quest_node(nullptr){}
	bool isUsing;
	bool isTerminated;
    QuestType* quest_node;
};

/// <summary>
/// 继承QuestType的用于线程池使用的线程节点，用于保存线程的状态
/// </summary>
class ThreadNode : public QuestType
{
    bool isOccupied;
    std::thread t;
};

/// <summary>
/// 一个简陋的线程池：
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
         * 换句话说，这个加载操作之后的所有操作（在当前线程中）都会看到这个加载操作的结果12。
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
    /// 将一个任务节点初始化它的特征后放入池中
    /// </summary>
    /// <param name="quest"></param>
    void AddQuestToPool(QuestType quest)
    {
        QuestNode *p_node = new QuestNode();
        p_node->quest_node = new QuestType();
        *p_node->quest_node = quest;
        QuestList.AddQuestToQueue(p_node);  
    }

    void ConsumeQuestFromPool()
    {

    }

private:
    EventsQueue QuestList;
    EventsQueue ThreadList;

    ShabbyThreadPool() { /*...*/ }
    ~ShabbyThreadPool() { /*...*/ }
    ShabbyThreadPool(const ShabbyThreadPool&) = delete;
    ShabbyThreadPool& operator=(const ShabbyThreadPool&) = delete;

    static std::atomic<ShabbyThreadPool*> instance;
    static std::mutex mutex;
};

// 在类外初始化静态成员
std::atomic<ShabbyThreadPool*> ShabbyThreadPool::instance;
std::mutex ShabbyThreadPool::mutex;

#endif // !SHABBY_THREAD_POOL_H
