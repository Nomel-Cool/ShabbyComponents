#pragma once
#ifndef EVENT_QUEUE_H
#define EVENT_QUEUE_H

#include <queue>
#include <mutex>
#include <memory>
#include <condition_variable>

/// <summary>
/// ��ƣ�
/// һ�Զ���̰߳�ȫ����
/// </summary>
template<typename T>
class TaskModelQueue 
{
public:
    TaskModelQueue() = default;
    ~TaskModelQueue() = default;
    TaskModelQueue(const TaskModelQueue& other) = delete;
    TaskModelQueue& operator=(const TaskModelQueue&) = delete;

    /// <summary>
    /// �����пգ������пչ�������ȡ�������������Ӱ��
    /// </summary>
    /// <returns>��Ϊtrue������false</returns>
    bool Empty() const
    {
        std::lock_guard<std::mutex> lock(const_cast<std::mutex&>(mutex_));
        return inner_queue.empty();
    }

    /// <summary>
    /// ����һ����Ϣ����Ժ󣬳��Ի�������һ�������Ļ�ȡ�����������Ƕ�������ߺ���������Queue��size=0�����ָ�
    /// </summary>
    /// <param name="enlisted_node">�¼�����ָ��</param>
    void AddQuestToQueue(std::unique_ptr<T> enlisted_node)
    {
        std::lock_guard<std::mutex> lock(mutex_);
        inner_queue.push(std::move(enlisted_node));
        cond_.notify_one();
    }

    /// <summary>
    /// �ӷǿն����л�ȡһ���¼���
    /// ����ǰ����Ϊ�գ��������ؼ�ʹ����������Ч��
    /// ����ǰ���в�Ϊ�գ����������棬����Ӳ������ѵĽ���������Ч��
    /// ����ȡ���¼������쳣��������std::move��֤�����ڵ�Ԫ�ؿ���Ȩ��ȫ���������������ط�����ʹ�á�
    /// </summary>
    /// <returns>����ͷ���¼�</returns>
    std::unique_ptr<T> GetQuestFromQueue()
    {
        std::unique_lock<std::mutex> lock(mutex_);
        cond_.wait(lock, [this] { return !inner_queue.empty(); });
        auto consumed_node = std::move(inner_queue.front());
        inner_queue.pop();
        return consumed_node;
    }

private:
    std::queue<std::unique_ptr<T>> inner_queue;
    std::mutex mutex_;
    std::condition_variable cond_;
};

#endif // !EVENT_QUEUE_H
