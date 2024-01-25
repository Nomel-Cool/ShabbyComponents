#pragma once
#ifndef QUEST_TYPE_H
#define QUEST_TYPE_H

#include <stack>
#include <map>
#include <functional>
#include <string>
#include <any>
#include <future>

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
    /// �����ذ汾������auto��ģ���������д����
    /// </summary>
    /// <param name="func_id">���ӱ�ǩid/param>
    /// <returns>�ɵ��ú���</returns>
    auto GetFunctor(std::string func_id)
    {
        try
        {
            return quest_type[func_id];
        }
        catch (std::bad_any_cast& ex)
        {
            // �����쳣
            throw ex;
        }
    }

    /// <summary>
    /// ͨ���������ƻ�ȡ��װ�ĺ������ʽ
    /// (TODO)Ӧ��ʹ��ö�����ӳ���ȡ����ID���������ĺô��ǿ��Թ�ϣ��֤Ψһ��
    /// �����ת��������ȫ��ǰ����������������ID��Ӧ�ĺ����ṹ������ͻ��׳��쳣
    /// </summary>
    /// <typeparam name="RETURN_TYPE">ģ�����: ����ֵ����</typeparam>
    /// <typeparam name="...ARGS_TYPE">ģ�����: �β�����</typeparam>
    /// <param name="func_id">������map�е�keyֵ</param>
    /// <returns>�ɵ��ú���</returns>
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
    /// Ϊ�˽���GetFunctor��ģ��ı�д
    /// </summary>
    /// <typeparam name="RETURN_TYPE"></typeparam>
    /// <typeparam name="...ARGS_TYPE"></typeparam>
    /// <param name="any_func">������map�е�keyֵ</param>
    /// <returns></returns>
    template<typename RETURN_TYPE,typename... ARGS_TYPE>
    std::function<RETURN_TYPE(ARGS_TYPE...)> CastAnyToFunction(std::any any_func)
    {
        return std::any_cast<std::function<RETURN_TYPE(ARGS_TYPE...)>>(any_func);
    }
    
    /// <summary>
    /// �����첽���ӵ�future
    /// </summary>
    /// <typeparam name="RETURN_TYPE">���ӷ���ֵ����</typeparam>
    /// <typeparam name="...ARGS_TYPE">���Ӳ����б�����</typeparam>
    /// <param name="any_asyncfunc">������map�е�keyֵ</param>
    /// <returns></returns>
    template<typename RETURN_TYPE, typename... ARGS_TYPE>
    std::future<RETURN_TYPE(ARGS_TYPE...)> CastAnyToFuture(std::any any_asyncfunc)
    {
        return std::any_cast<std::future<RETURN_TYPE(ARGS_TYPE...)>>(any_asyncfunc);
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

    /// <summary>
    /// ���ں��ӵ�ʹ�û����첽�����ʹ��std::promise����ǩ�����ӵĲ��������Ϊstd::future
    /// </summary>
    /// <typeparam name="RETURN_TYPE">���ӷ���ֵ����</typeparam>
    /// <typeparam name="...ARGS_TYPE">���Ӳ����б�����</typeparam>
    /// <param name="func_id">������map�е�keyֵ</param>
    /// <param name="any_asyncfunc">��������������дlambda</param>
    template<typename RETURN_TYPE, typename... ARGS_TYPE>
    void SetAsyncFunctor(std::string func_id, std::promise<RETURN_TYPE(ARGS_TYPE...)> any_asyncfunc)
    {
        quest_type[func_id] = any_asyncfunc;
    }


private:
    std::map<std::string, std::any> quest_type;
};

#endif // !QUEST_TYPE_H