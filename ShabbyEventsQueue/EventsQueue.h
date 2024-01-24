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
	void AddQuestToQueue(const QuestType* enlisted_node)
	{

		inner_queue.push(*enlisted_node);
	}
	QuestType* GetQuestFromQueue()
	{
		if (!inner_queue.empty())
		{
			auto consumed_node = inner_queue.front();
			inner_queue.pop();
			return new QuestType(consumed_node);
		}
		return nullptr;
	}
private:
	std::queue<QuestType> inner_queue;
};

#endif // !EVENT_QUEUE_H
