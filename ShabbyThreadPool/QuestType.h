#pragma once
#ifndef QUEST_TYPE_H
#define QUEST_TYPE_H

#include <stack>
#include <map>
#include <functional>
#include <string>
#include <any>

/// <summary>
/// 一种回调函数操作类
/// 尝试一种骚操作，返回值和参数都是同一个类的指针
/// 即一个类同时定义了返回值和参数
/// 函数的解释权全部收敛到一个类当中，只需要设置好QuestType中的函数表示，再传入这个QuestType对象即可。
/// 
/// 实现技术重点：
/// 1. 可变模板参数
/// 2. std::any
/// </summary>
class QuestType
{
public:
    /// <summary>
    /// 通过函子名称获取封装的函数表达式
    /// (TODO)应当使用枚举体加映射获取函子ID，这样做的好处是可以哈希保证唯一性
    /// 这里的转换并不安全，前提是你必须清楚函子ID对应的函数结构，否则就会抛出异常
    /// </summary>
    /// <param name="func_id">函子字符串名称</param>
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
            // 处理异常
            throw ex;
        }
    }


    /// <summary>
    /// 设置一个函子的格式，返回值 + 参数列表
    /// </summary>
    /// <typeparam name="RETURN_TYPE">函子返回值类型</typeparam>
    /// <typeparam name="...ARGS_TYPE">函子参数列表类型</typeparam>
    /// <param name="func_id">函子在map中的key值</param>
    /// <param name="func">函数本身，建议填写lambda</param>
    template<typename RETURN_TYPE, typename... ARGS_TYPE>
    void SetFunctor(std::string func_id, std::function<RETURN_TYPE(ARGS_TYPE...)> func)
    {
        quest_type[func_id] = func;
    }


private:
    std::map<std::string, std::any> quest_type;
};

#endif // !QUEST_TYPE_H