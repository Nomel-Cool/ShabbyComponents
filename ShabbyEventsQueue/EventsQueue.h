#pragma once
#ifndef EVENT_QUEUE_H
#define EVENT_QUEUE_H

#include <queue>
#include <mutex>
#include <condition_variable>

#include "../ShabbyUniversalType/QuestType.h"

/// <summary>
/// 设计：
/// 使用T作为通用事件的队列，任何事态都视为函数
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
	/// 将一个事件常量入队
	/// </summary>
	/// <param name="enlisted_node">事件常量指针</param>
	void AddQuestToQueue(T enlisted_node)
	{
		std::lock_guard<std::mutex> lock(mutex_);
		inner_queue.push(enlisted_node);
		// 当有消息入队以后，尝试唤醒阻塞的获取操作，核心是消费者和生产者由Queue的size=0这个点分隔
		cond_.notify_all();
	}

	/// <summary>
	/// 获取队列头部事件
	/// </summary>
	/// <returns>返回头部事件的指针</returns>
	T* GetQuestFromQueue()
	{
		// 获取锁
		std::unique_lock<std::mutex> lock(mutex_);
		// 队列为空，那么当前获取操作将阻塞，当有消息入队后就被唤醒
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
