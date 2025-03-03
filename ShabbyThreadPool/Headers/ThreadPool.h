#pragma once

#ifndef SHABBY_THREAD_POOL_EXPORTS
#define SHABBY_CLASS __declspec(dllimport)
#define SHABBY_TEMPLATE
#else //EXPORT
#define SHABBY_CLASS __declspec(dllexport)
#define SHABBY_TEMPLATE __declspec(dllexport)
#endif

#include <vector>
#include <thread>
#include <atomic>
#include <functional>
#include <future>

#include "EventsQueue.h"

class SHABBY_CLASS ShabbyThreadPool {
public:
    // ��ȡ����ʵ��
    static ShabbyThreadPool& GetInstance();

    /// <summary>
    /// ����F(Args...)�Ŀɵ������ͣ���ǩ��һ��F()��future��get_future��Ϊ����ֵ
    /// </summary>
    /// <typeparam name="F">�ɵ������ͣ�������lambda��function<T>������</typeparam>
    /// <typeparam name="...Args">���ģ��</typeparam>
    /// <param name="f">�ɵ������Ͷ���</param>
    /// <param name="...args">�ɵ������Ͷ���Ķ�Ӧ�����б�</param>
    /// <returns></returns>
    template<typename F, typename... Args>
    auto Enqueue(F&& f, Args&&... args) -> std::future<typename std::invoke_result_t<F, Args...>> {
        using return_type = typename std::invoke_result_t<F, Args...>;
        // ��������shared_ptrָ�򣬵�����ʹ��lambda����󣬰�����lambdaתΪunique_ptr����������
        auto task = std::make_shared<std::packaged_task<return_type()>>(std::bind(std::forward<F>(f), std::forward<Args>(args)...));
        std::future<return_type> res = task->get_future();
        // ʹ�� std::function ������ shared_ptr �Ա��⿽������
        auto wrapped_task = [task]() { (*task)(); };
        task_queue.AddQuestToQueue(std::make_unique<std::function<void()>>(std::move(wrapped_task)));
        return res;
    }
    // �̳߳�����
    ~ShabbyThreadPool();

private:
    ShabbyThreadPool(size_t numThreads); // ���캯��˽�л�
    ShabbyThreadPool(const ShabbyThreadPool&) = delete; // ɾ����������
    ShabbyThreadPool& operator=(const ShabbyThreadPool&) = delete; // ɾ����ֵ����
    void worker(); // �����̺߳���
    TaskModelQueue<std::function<void()>> task_queue; // ʹ���¼����й�������
    std::vector<std::thread> workers;
    std::atomic<bool> stop;
};
