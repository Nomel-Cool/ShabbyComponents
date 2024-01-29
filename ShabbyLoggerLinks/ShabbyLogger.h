#pragma once
#ifndef SHABBY_LOGGER_H
#define SHAABY_LOGGER_H

#include "../ShabbyUniversalType/QuestType.h"

namespace shabby
{
	enum LogDirection
	{
		ToFile,
		ToDataBase,
	};
	/// <summary>
	/// �ṩ��������־�ӿڣ�
	/// 1.д���ļ�
	/// 2.д�����ݿ⣨SQLlite��
	/// 3.��ѯ�ļ�
	/// 4.��ѯ���ݿ�
	/// </summary>
	class LogBase
	{
	public:
		LogBase() {}
		virtual ~LogBase() {}
		virtual void TakeALog(QuestType msg) = 0;
		virtual std::vector<QuestType> QueryLogsFrom(LogDirection log_repo) = 0;
	private:
	};
}

#endif