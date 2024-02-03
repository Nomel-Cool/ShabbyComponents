#pragma once
#ifndef SHABBY_LOGGER_H
#define SHAABY_LOGGER_H

#include <chrono>
#include <thread>

#include "../ShabbyUniversalType/QuestType.h"
#include "../ShabbyEventsQueue/EventsQueue.h"
#include "../ShabbyThreadPool/ShabbyThreadPool.h"
#include "../ShabbyAlgorithmLibrary/Compressing.h"

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
		virtual void TakeALog(LogDirection log_destination, QuestType msg) = 0;
		virtual std::vector<QuestType> QueryLogsFrom(LogDirection log_repo) = 0;
	protected:
		EventsQueue<QuestType> LogQueue;
	private:
	};
	class DefaultLogger : public LogBase
	{
	public:
		DefaultLogger(){}
		DefaultLogger(ShabbyThreadPool* pool_instance, LogDirection log_location, uint32_t poll_interval)
		{
			QuestType invoke_log_queue;
			invoke_log_queue.SetFunctor<void>("InvokeLogQueue", std::function<void()>(
				[=](){InvokeLogQueue(log_location,poll_interval); }
			));
			// (MARK)一个风险点就是如果线程池无法获取，导致创建元素阻塞会很麻烦
			pool_instance->AddQuestToPool<void, LogDirection, uint32_t>("Logger", invoke_log_queue);
		}

		virtual ~DefaultLogger(){}
		/// <summary>
		/// 轮询队列，如果有元素则消耗
		/// 1.外部定义一个携带日志信息的QuestType，与写入目的地
		/// 2.外部使用线程池调用该函数
		/// </summary>
		/// <param name="log_destination">日志记录目的地</param>
		/// <param name="poll_interval">轮询周期</param>
		virtual void InvokeLogQueue(LogDirection log_destination, uint32_t poll_interval)
		{
			while (true)
			{
				// 简易倒计时
				uint32_t reserved_interval = poll_interval;
				while (poll_interval > 0)
				{
					std::this_thread::sleep_for(std::chrono::milliseconds(1));
					--poll_interval;
				}
				auto msg = LogQueue.GetQuestFromQueue();
				// TODO,“GetLogMessage”应当统一管理
				RedirectTo(log_destination, msg->GetFunctor<std::string>("GetLogMessage")());
				poll_interval = reserved_interval;
			}
		}
	protected:
		/// <summary>
		/// 记录一个日志事件到目的地
		/// 日志格式：时间 记录所在的函数 发生事件信息
		/// </summary>
		/// <param name="log_destination">日志目的地</param>
		/// <param name="msg">日志信息</param>
		virtual void TakeALog(LogDirection log_destination, QuestType msg) override
		{
			LogQueue.AddQuestToQueue(msg);
		}
		/// <summary>
		/// 重定向到文件或者是数据库，需要文件管理工程的支持以及数据库管理工程的支持
		/// </summary>
		/// <param name="log_destination">日志写入终端</param>
		/// <param name="msg">日志信息</param>
		void RedirectTo(LogDirection log_destination, std::string msg)
		{
			// 加入压缩算法
			TextHuffmanCompressing huffman_compressing;
			huffman_compressing.BuildHuffmanTree(msg);
			auto huffman_dictionary = huffman_compressing.GetHuffmanCode();
			std::string compressed_str = "";
			for (const char& c : msg)
			{
				compressed_str += huffman_dictionary[c];
			}
			// 判断日志输出位置，选择使用文件还是数据库管理控件
		}
	private:

	};
}

#endif