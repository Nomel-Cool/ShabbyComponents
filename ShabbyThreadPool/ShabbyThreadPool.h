#pragma once
#ifndef SHABBY_THREAD_POOL_H
#define SHABBY_THREAD_POOL_H

#include <thread>
#include <algorithm>
#include <functional>
#include <memory>

#include "../ShabbyUniversalType/QuestType.h"

class QuestNode
{
public:
	QuestNode():isUsing(false),isTerminated(false)
	{
		quest_node = new QuestType();
	}

	QuestType* GetInstance()
	{
		if (quest_node != nullptr)
			return quest_node;
		return nullptr;
	}
	bool isUsing;
	bool isTerminated;
private:
	QuestType* quest_node;

};

/// <summary>
/// 一个简陋的线程池：
/// 组成：
/// 一个有限线程元容器
/// 一个无限的任务队列
/// 操作：
/// 任务以队列方式与线程元关联后被消费
/// 线程元被任务消费过程中需要加锁，同时任务完成前也需要加锁
/// 
/// </summary>
class ShabbyThreadPool
{
public:
	ShabbyThreadPool()
	{
		node = new QuestNode();
	}
	~ShabbyThreadPool()
	{

	}
	QuestNode* GetInstance()
	{
		if (node != nullptr)
			return node;
		return nullptr;
	}
	bool AddQuestWithThread()
	{

	}
private:
	QuestNode *node;
};

#endif // !SHABBY_THREAD_POOL_H
