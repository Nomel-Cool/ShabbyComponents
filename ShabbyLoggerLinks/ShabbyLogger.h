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
		virtual void TakeALog(QuestType msg) = 0;
		virtual std::vector<QuestType> QueryLogsFrom(LogDirection log_repo) = 0;
	private:
	};
}

#endif