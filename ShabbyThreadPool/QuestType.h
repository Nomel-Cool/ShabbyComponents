#pragma once
#ifndef QUEST_TYPE_H
#define QUEST_TYPE_H

#include <stack>
#include <map>
#include <functional>
#include <string>
#include <any>

/// <summary>
/// һ�ֻص�����������
/// ����һ��ɧ����������ֵ�Ͳ�������ͬһ�����ָ��
/// ��һ����ͬʱ�����˷���ֵ�Ͳ���
/// �����Ľ���Ȩȫ��������һ���൱�У�ֻ��Ҫ���ú�QuestType�еĺ�����ʾ���ٴ������QuestType���󼴿ɡ�
/// 
/// ʵ�ּ����ص㣺
/// 1. �ɱ�ģ�����
/// 2. std::any
/// </summary>
class QuestType
{
public:
    /// <summary>
    /// ͨ���������ƻ�ȡ��װ�ĺ������ʽ
    /// (TODO)Ӧ��ʹ��ö�����ӳ���ȡ����ID���������ĺô��ǿ��Թ�ϣ��֤Ψһ��
    /// �����ת��������ȫ��ǰ����������������ID��Ӧ�ĺ����ṹ������ͻ��׳��쳣
    /// </summary>
    /// <param name="func_id">�����ַ�������</param>
    /// <returns></returns>
    template<typename RETURN_TYPE, typename... ARGS_TYPE>
    std::function<RETURN_TYPE(ARGS_TYPE...)> GetFunctor(std::string func_id)
    {
        try
        {
            return std::any_cast<std::function<RETURN_TYPE(ARGS_TYPE...)>>(quest_type[func_id]);
        }
        catch (std::bad_any_cast& ex)
        {
            // �����쳣
            throw ex;
        }
    }


    /// <summary>
    /// ����һ�����ӵĸ�ʽ������ֵ + �����б�
    /// </summary>
    /// <typeparam name="RETURN_TYPE">���ӷ���ֵ����</typeparam>
    /// <typeparam name="...ARGS_TYPE">���Ӳ����б�����</typeparam>
    /// <param name="func_id">������map�е�keyֵ</param>
    /// <param name="func">��������������дlambda</param>
    template<typename RETURN_TYPE, typename... ARGS_TYPE>
    void SetFunctor(std::string func_id, std::function<RETURN_TYPE(ARGS_TYPE...)> func)
    {
        quest_type[func_id] = func;
    }


private:
    std::map<std::string, std::any> quest_type;
};

#endif // !QUEST_TYPE_H