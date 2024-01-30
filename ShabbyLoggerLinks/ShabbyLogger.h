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
	/// 
	/// 一致性：
	/// virtual  +  协变返回值  +  函数名  +  参数列表  =>  Override  // 使用子类函数，除非使用using显示调用父类函数，否则是不可见的，会动态绑定
	///                           函数名             =>  Hidden    // 谁调用就用谁的版本，与继承无关;相当于自己的一个域名下的函数
	/// 
	/// 不是很复杂的事情，但凡有virtual的都是优先看向派生类的，然后通过动态绑定有可能回到父类
	/// 没有virtual的认为是加了自己类名域额外签名的函数，谁调用就用谁的版本，除非显式使用using更改它的域
	/// 
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