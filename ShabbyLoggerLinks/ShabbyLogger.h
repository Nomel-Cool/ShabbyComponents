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
	/// �ṩ��������־�ӿڣ�
	/// 1.д���ļ�
	/// 2.д�����ݿ⣨SQLlite��
	/// 3.��ѯ�ļ�
	/// 4.��ѯ���ݿ�
	/// </summary>
	/// 
	/// һ���ԣ�
	/// virtual  +  Э�䷵��ֵ  +  ������  +  �����б�  =>  Override  // ʹ�����ຯ��������ʹ��using��ʾ���ø��ຯ���������ǲ��ɼ��ģ��ᶯ̬��
	///                           ������             =>  Hidden    // ˭���þ���˭�İ汾����̳��޹�;�൱���Լ���һ�������µĺ���
	/// 
	/// ���Ǻܸ��ӵ����飬������virtual�Ķ������ȿ���������ģ�Ȼ��ͨ����̬���п��ܻص�����
	/// û��virtual����Ϊ�Ǽ����Լ����������ǩ���ĺ�����˭���þ���˭�İ汾��������ʽʹ��using����������
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
			// (MARK)һ�����յ��������̳߳��޷���ȡ�����´���Ԫ����������鷳
			pool_instance->AddQuestToPool<void, LogDirection, uint32_t>("Logger", invoke_log_queue);
		}

		virtual ~DefaultLogger(){}
		/// <summary>
		/// ��ѯ���У������Ԫ��������
		/// 1.�ⲿ����һ��Я����־��Ϣ��QuestType����д��Ŀ�ĵ�
		/// 2.�ⲿʹ���̳߳ص��øú���
		/// </summary>
		/// <param name="log_destination">��־��¼Ŀ�ĵ�</param>
		/// <param name="poll_interval">��ѯ����</param>
		virtual void InvokeLogQueue(LogDirection log_destination, uint32_t poll_interval)
		{
			while (true)
			{
				// ���׵���ʱ
				uint32_t reserved_interval = poll_interval;
				while (poll_interval > 0)
				{
					std::this_thread::sleep_for(std::chrono::milliseconds(1));
					--poll_interval;
				}
				auto msg = LogQueue.GetQuestFromQueue();
				// TODO,��GetLogMessage��Ӧ��ͳһ����
				RedirectTo(log_destination, msg->GetFunctor<std::string>("GetLogMessage")());
				poll_interval = reserved_interval;
			}
		}
	protected:
		/// <summary>
		/// ��¼һ����־�¼���Ŀ�ĵ�
		/// ��־��ʽ��ʱ�� ��¼���ڵĺ��� �����¼���Ϣ
		/// </summary>
		/// <param name="log_destination">��־Ŀ�ĵ�</param>
		/// <param name="msg">��־��Ϣ</param>
		virtual void TakeALog(LogDirection log_destination, QuestType msg) override
		{
			LogQueue.AddQuestToQueue(msg);
		}
		/// <summary>
		/// �ض����ļ����������ݿ⣬��Ҫ�ļ������̵�֧���Լ����ݿ�����̵�֧��
		/// </summary>
		/// <param name="log_destination">��־д���ն�</param>
		/// <param name="msg">��־��Ϣ</param>
		void RedirectTo(LogDirection log_destination, std::string msg)
		{
			// ����ѹ���㷨
			TextHuffmanCompressing huffman_compressing;
			huffman_compressing.BuildHuffmanTree(msg);
			auto huffman_dictionary = huffman_compressing.GetHuffmanCode();
			std::string compressed_str = "";
			for (const char& c : msg)
			{
				compressed_str += huffman_dictionary[c];
			}
			// �ж���־���λ�ã�ѡ��ʹ���ļ��������ݿ����ؼ�
		}
	private:

	};
}

#endif