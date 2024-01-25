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
    /// （重载版本）利用auto把模板参数的填写解耦
    /// </summary>
    /// <param name="func_id">函子标签id/param>
    /// <returns>可调用函数</returns>
    auto GetFunctor(std::string func_id)
    {
        try
        {
            return quest_type[func_id];
        }
        catch (std::bad_any_cast& ex)
        {
            // 处理异常
            throw ex;
        }
    }

    /// <summary>
    /// 通过函子名称获取封装的函数表达式
    /// (TODO)应当使用枚举体加映射获取函子ID，这样做的好处是可以哈希保证唯一性
    /// 这里的转换并不安全，前提是你必须清楚函子ID对应的函数结构，否则就会抛出异常
    /// </summary>
    /// <typeparam name="RETURN_TYPE">模板参数: 返回值类型</typeparam>
    /// <typeparam name="...ARGS_TYPE">模板参数: 形参类型</typeparam>
    /// <param name="func_id">函子在map中的key值</param>
    /// <returns>可调用函数</returns>
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
    /// 为了解耦GetFunctor与模板的编写
    /// </summary>
    /// <typeparam name="RETURN_TYPE"></typeparam>
    /// <typeparam name="...ARGS_TYPE"></typeparam>
    /// <param name="any_func">函子在map中的key值</param>
    /// <returns></returns>
    template<typename RETURN_TYPE,typename... ARGS_TYPE>
    std::function<RETURN_TYPE(ARGS_TYPE...)> CastAnyToFunction(std::any any_func)
    {
        return std::any_cast<std::function<RETURN_TYPE(ARGS_TYPE...)>>(any_func);
    }
    
    /// <summary>
    /// 返回异步函子的future
    /// </summary>
    /// <typeparam name="RETURN_TYPE">函子返回值类型</typeparam>
    /// <typeparam name="...ARGS_TYPE">函子参数列表类型</typeparam>
    /// <param name="any_asyncfunc">函子在map中的key值</param>
    /// <returns></returns>
    template<typename RETURN_TYPE, typename... ARGS_TYPE>
    std::future<RETURN_TYPE(ARGS_TYPE...)> CastAnyToFuture(std::any any_asyncfunc)
    {
        return std::any_cast<std::future<RETURN_TYPE(ARGS_TYPE...)>>(any_asyncfunc);
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

    /// <summary>
    /// 鉴于函子的使用还有异步，因此使用std::promise负责签发函子的操作结果作为std::future
    /// </summary>
    /// <typeparam name="RETURN_TYPE">函子返回值类型</typeparam>
    /// <typeparam name="...ARGS_TYPE">函子参数列表类型</typeparam>
    /// <param name="func_id">函子在map中的key值</param>
    /// <param name="any_asyncfunc">函数本身，建议填写lambda</param>
    template<typename RETURN_TYPE, typename... ARGS_TYPE>
    void SetAsyncFunctor(std::string func_id, std::promise<RETURN_TYPE(ARGS_TYPE...)> any_asyncfunc)
    {
        quest_type[func_id] = any_asyncfunc;
    }


private:
    std::map<std::string, std::any> quest_type;
};

#endif // !QUEST_TYPE_H