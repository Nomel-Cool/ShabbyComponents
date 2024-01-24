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
/// �̳�QuestType�������̳߳ص��õ��¼��ڵ��࣬���ڼ�¼�ڵ㱣�浽�̳߳�
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
/// �̳�QuestType�������̳߳�ʹ�õ��߳̽ڵ㣬���ڱ����̵߳�״̬
/// </summary>
class ThreadNode : public QuestType
{
    bool isOccupied;
    std::thread t;
};

/// <summary>
/// һ����ª���̳߳أ�
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
         * ���仰˵��������ز���֮������в������ڵ�ǰ�߳��У����ῴ��������ز����Ľ��12��
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
    /// ��һ������ڵ��ʼ������������������
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

// �������ʼ����̬��Ա
std::atomic<ShabbyThreadPool*> ShabbyThreadPool::instance;
std::mutex ShabbyThreadPool::mutex;

#endif // !SHABBY_THREAD_POOL_H
