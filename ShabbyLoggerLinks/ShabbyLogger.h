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
	/// 提供基本的日志接口：
	/// 1.写入文件
	/// 2.写入数据库（SQLlite）
	/// 3.查询文件
	/// 4.查询数据库
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