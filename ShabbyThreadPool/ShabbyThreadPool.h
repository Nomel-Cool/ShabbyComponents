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
        ThreadNode() {}
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
    /// ԭ��
    /// ����������̰߳�ȫ�ģ����������̳߳أ�������������ڵ�
    /// ����ڵ���ӵ��̳߳ػ����̳߳�
    /// �̳߳��еġ�ÿһ���̶߳��������������������С�����ѭ��״̬
    /// 
    /// ������ת��
    /// 1.��������ڵ�
    /// 2.�̳߳�ĳһ�߳������õ�����ڵ㲢���������Լ���ִ��
    /// 3.�߳�ִ��������ȡ������󣬼���Ͷ������������
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
            std::unique_lock lock(quest_mutex);
            QuestNode p_node(async_quest);
            p_node.quest_id = quest_id;
            QuestList.AddQuestToQueue(p_node);
            _cond.notify_one(); // ��������м�������ʱ�����߳�������������
        }

        /// <summary>
        /// ���������������һ������
        /// </summary>
        /// <typeparam name="RETURN_TYPE">���ӷ���ֵ�������ǻص�Ӧ��Ϊvoid</typeparam>
        /// <typeparam name="...ARGS_TYPE">���ӵ��ò��������б�</typeparam>
        /// <return>��������ָ��</return>
        std::shared_ptr<std::packaged_task<QuestType()>> ConsumeQuestFromPool()
        {
            std::unique_lock lock(quest_mutex);
            while (QuestList.Empty()) // �������߳�Ϊ��ʱ ���� �����б�Ϊ��ʱ ���޷�������ִ������
                _cond.wait(lock); // �ڵȴ�ʱ���Զ��ͷ���
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
                UpdateBusyWatcher(thread_index); // ����ʼʱ
                (*p_func)();
                UpdateBusyWatcher(thread_index); // ������ɺ�
            }
        }

        /// <summary>
        /// ֮ǰһֱ�����ˣ��̳߳ص�ÿ���߳����������Լ�������ľ��ǲ�ͣ�ػ�ȡ������е�����
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
        /// �ͷ��̳߳������̰߳�������
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
            std::lock_guard lock(watcher_mutex);
            BusyWatcher ^= (1 << using_index); // ����using_indexλ���1���������0
        }
    };

    // �������ʼ����̬��Ա
    std::atomic<ShabbyThreadPool*> ShabbyThreadPool::instance;
    std::mutex ShabbyThreadPool::init_mutex;
}
#endif // !SHABBY_THREAD_POOL_H
