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
/// һ����ª���̳߳أ�
/// ��ɣ�
/// һ�������߳�Ԫ����
/// һ�����޵��������
/// ������
/// �����Զ��з�ʽ���߳�Ԫ����������
/// �߳�Ԫ���������ѹ�������Ҫ������ͬʱ�������ǰҲ��Ҫ����
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
