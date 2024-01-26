#pragma once
#ifndef EVENT_QUEUE_H
#define EVENT_QUEUE_H

#include <queue>
#include <mutex>
#include <condition_variable>

#include "../ShabbyUniversalType/QuestType.h"

/// <summary>
/// ��ƣ�
/// ʹ��T��Ϊͨ���¼��Ķ��У��κ���̬����Ϊ����
/// 
/// </summary>
template<typename T>
class EventsQueue
{
public:
	EventsQueue()
	{

	}
	~EventsQueue()
	{

	}

	bool Empty()
	{
		std::lock_guard<std::mutex> lock(mutex_);
		return inner_queue.empty();
	}

	/// <summary>
	/// ��һ���¼��������
	/// </summary>
	/// <param name="enlisted_node">�¼�����ָ��</param>
	void AddQuestToQueue(T enlisted_node)
	{
		std::lock_guard<std::mutex> lock(mutex_);
		inner_queue.push(enlisted_node);
		// ������Ϣ����Ժ󣬳��Ի��������Ļ�ȡ�����������������ߺ���������Queue��size=0�����ָ�
		cond_.notify_all();
	}

	/// <summary>
	/// ��ȡ����ͷ���¼�
	/// </summary>
	/// <returns>����ͷ���¼���ָ��</returns>
	T* GetQuestFromQueue()
	{
		// ��ȡ��
		std::unique_lock<std::mutex> lock(mutex_);
		// ����Ϊ�գ���ô��ǰ��ȡ������������������Ϣ��Ӻ�ͱ�����
		while (inner_queue.empty())
			cond_.wait(lock);
		auto consumed_node = inner_queue.front();
		inner_queue.pop();
		return new T(consumed_node);
	}
private:
	std::queue<T> inner_queue;
	std::mutex mutex_;
	std::condition_variable cond_;
};

#endif // !EVENT_QUEUE_H
