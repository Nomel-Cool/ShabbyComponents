#pragma once
#ifndef EVENT_QUEUE_H
#define EVENT_QUEUE_H

#include <queue>
#include <mutex>
#include <condition_variable>

#include "../ShabbyUniversalType/QuestType.h"

/// <summary>
/// ��ƣ�
/// ʹ��QuestType��Ϊͨ���¼��Ķ��У��κ���̬����Ϊ����
/// 
/// </summary>
class EventsQueue
{
public:
	EventsQueue()
	{

	}
	~EventsQueue()
	{

	}

	/// <summary>
	/// ��һ���¼��������
	/// </summary>
	/// <param name="enlisted_node">�¼�����ָ��</param>
	void AddQuestToQueue(QuestType* enlisted_node)
	{
		std::lock_guard<std::mutex> lock(mutex_);
		inner_queue.push(*enlisted_node);
		// ������Ϣ����Ժ󣬳��Ի��������Ļ�ȡ�����������������ߺ���������Queue��size=0�����ָ�
		cond_.notify_all();
	}

	/// <summary>
	/// ��ȡ����ͷ���¼�
	/// </summary>
	/// <returns>����ͷ���¼���ָ��</returns>
	QuestType* GetQuestFromQueue()
	{
		// ��ȡ��
		std::unique_lock<std::mutex> lock(mutex_);
		// ����Ϊ�գ���ô��ǰ��ȡ������������������Ϣ��Ӻ�ͱ�����
		while (inner_queue.empty())
			cond_.wait(lock);
		auto consumed_node = inner_queue.front();
		inner_queue.pop();
		return new QuestType(consumed_node);
	}
private:
	std::queue<QuestType> inner_queue;
	std::mutex mutex_;
	std::condition_variable cond_;
};

#endif // !EVENT_QUEUE_H
