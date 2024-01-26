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
/// �̳�QuestType�������̳߳ص��õ��¼��ڵ��࣬���ڼ�¼�ڵ㱣�浽�̳߳�
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
/// �̳�QuestType�������̳߳�ʹ�õ��߳̽ڵ㣬���ڱ����̵߳�״̬
/// </summary>
class ThreadNode
{
public:
    ThreadNode() :isBusy(false) {}
    bool isBusy;
    std::thread thread_node;
};

/// <summary>
/// һ����ª���̳߳أ�
/// 
/// �����:
/// �߳��������ȴ���: �������б�Ϊ��ʱ
/// �߳����������ѡ�: �����б��������ڵ�ʱ
/// 
/// ��ɣ�
/// һ�������߳�Ԫ����
/// һ�����޵��������
/// ������
/// �����Զ��з�ʽ���߳�Ԫ����������
/// �߳�Ԫ���������ѹ�������Ҫ������ͬʱ�������ǰҲ��Ҫ����
/// ֧�������ͺ������������û���ڹ涨�����±�ִ�У������ͺ��ȡ����
/// 
/// </summary>
class ShabbyThreadPool
{
public:
    static ShabbyThreadPool* GetPoolInstance()
    {
        /*
         * std::memory_order_acquire����ڴ�˳���ǡ�
         * �����ǵĺ����ǣ���������ز���֮������ж�������д���������ܱ���������������ز���֮ǰ12��
         * ���仰˵��������ز���֮������в������ڵ�ǰ�߳��У����ῴ��������ز����Ľ����
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
    /// �ع��̳߳�����
    /// </summary>
    /// <param name="thread_amount">�̳߳����߳�����</param>
    /// <returns>�������Ĳ������</returns>
    void ResizePool(int thread_amount)
    {
        ThreadList.resize(thread_amount);
        BusyWatcher = (int)pow(2, thread_amount) - 1;
    }

    /// <summary>
    /// ��һ������ڵ��ʼ�����������������У�Ȼ���ȡ�߳�ִ�н��ƾ֤
    /// </summary>
    /// <param name="quest">Ҫ���첽����</param>
    template<typename RETURN_TYPE, typename... ARGS_TYPE>
    void AddQuestToPool(std::string quest_id, QuestType async_quest)
    {
        std::unique_lock lock(_mutex);
        QuestNode p_node(async_quest);
        p_node.quest_id = quest_id;
        QuestList.AddQuestToQueue(p_node);
        _cond.notify_all(); // ��������м�������ʱ�����߳�������������
    }

    /// <summary>
    /// ���������������һ������
    /// </summary>
    /// <typeparam name="RETURN_TYPE">���ӷ���ֵ�������ǻص�Ӧ��Ϊvoid</typeparam>
    /// <typeparam name="...ARGS_TYPE">���ӵ��ò��������б�</typeparam>
    void ConsumeQuestFromPool()
    {
        std::unique_lock lock(_mutex);
        while (ConvertBusyWatcherToIndex() == -1 || QuestList.Empty()) // �������߳�Ϊ��ʱ ���� �����б�Ϊ��ʱ ���޷�������ִ������
            _cond.wait(lock); // �ڵȴ�ʱ���Զ��ͷ���
        size_t available_thread_index = ConvertBusyWatcherToIndex();
        auto quest_node = QuestList.GetQuestFromQueue();
        auto async_func = quest_node->GetAsyncFunctor(quest_node->quest_id);
        auto func = quest_node->CastAnyToPromise<QuestType>(async_func);
        if (func == nullptr)
        {
            std::cout << "Empty func" << std::endl;
            return;
        }
        (*func)();
    }

    /// <summary>
    /// �̳߳����У�ִ������
    /// </summary>
    void LaunchPool()
    {
        std::thread _t(
            [this]()
            {
                stop_flag = false;
                while (!stop_flag)
                {
                    ConsumeQuestFromPool();
                }
            }
        );
        _t.detach();
    }

    /// <summary>
    /// �ͷ��̳߳������̰߳�������
    /// </summary>
    void StopPool()
    {
        stop_flag = true;
        for(auto& t : ThreadList)
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
    std::mutex _mutex;

    ShabbyThreadPool() { /*...*/ }
    ~ShabbyThreadPool() { /*...*/ }
    ShabbyThreadPool(const ShabbyThreadPool&) = delete;
    ShabbyThreadPool& operator=(const ShabbyThreadPool&) = delete;

    size_t ConvertBusyWatcherToIndex()
    {
        unsigned long index;
        if (_BitScanForward(&index, BusyWatcher)) // ΢��汾����GCC��Clang�У������ʹ��__builtin_ffs�������ﵽͬ����Ч��
        {
            BusyWatcher &= ~(1 << index); // ���ҵ���λ����Ϊ0
            return index;
        }
        return -1; // ������е�λ����0������-1��ʾû�п��õ��߳�
    }

    /// <summary>
    /// ���µ�ǰ�߳̿���״̬
    /// </summary>
    /// <param name="using_index">��ʹ���߳�index</param>
    void UpdateBusyWatcher(int using_index)
    {
        BusyWatcher &= ~(1 << using_index); // ����using_indexλ����Ϊ0
    }

    static std::atomic<ShabbyThreadPool*> instance;
    static std::mutex mutex;
};

// �������ʼ����̬��Ա
std::atomic<ShabbyThreadPool*> ShabbyThreadPool::instance;
std::mutex ShabbyThreadPool::mutex;

#endif // !SHABBY_THREAD_POOL_H
