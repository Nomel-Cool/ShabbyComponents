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
    /// 获取异步
    /// </summary>
    /// <param name="func_id">函子标签id</param>
    /// <returns>可委托调用函数</returns>
    auto GetAsyncFunctor(std::string func_id)
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
    /// 通过函子特征获取封装的函数表达式
    /// (TODO)应当使用枚举体加映射获取函子ID，这样做的好处是可以哈希保证唯一性
    /// 这里的转换并不安全，前提是你必须清楚函子ID对应的函数结构，否则就会抛出异常
    /// </summary>
    /// <typeparam name="RETURN_TYPE">返回值类型</typeparam>
    /// <typeparam name="...ARGS_TYPE">形参类型</typeparam>
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
    /// 通过函子特征获取封装的函数表达式
    /// </summary>
    /// <typeparam name="RETURN_TYPE">返回值类型</typeparam>
    /// <typeparam name="...ARGS_TYPE">形参类型</typeparam>
    /// <param name="func_id">函子在map中的key值</param>
    /// <returns>可委托调用函数</returns>
    template<typename RETURN_TYPE, typename... ARGS_TYPE>
    std::promise<RETURN_TYPE(ARGS_TYPE...)> GetAsyncFunctor(std::string func_id)
    {
        try
        {
            return std::any_cast<std::promise<RETURN_TYPE(ARGS_TYPE...)>>(quest_type[func_id]);
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
    template<typename RETURN_TYPE>
    auto CastAnyToPromise(std::any any_asyncfunc)
    {
        return std::any_cast<std::shared_ptr<std::packaged_task<RETURN_TYPE()>>>(any_asyncfunc);
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
    /// 用户将设置一个std::bind用于延迟计算，并及时签发一个future
    /// </summary>
    /// <typeparam name="RETURN_TYPE">函子返回值类型</typeparam>
    /// <typeparam name="...ARGS_TYPE">函子参数列表类型</typeparam>
    /// <param name="func_id">函子在map中的key值</param>
    /// <param name="func">函数本身，建议填写lambda</param>
    /// <returns>封装好延迟计算的函子</returns>
    template<typename RETURN_TYPE, typename... ARGS_TYPE>
    auto SetAsyncFunctor(std::string func_id, std::function<RETURN_TYPE(ARGS_TYPE...)> async_func, ARGS_TYPE... args)
    {
        //由于是延迟计算，只告诉了委托方调用时使用的参数，然后在未来获取计算值即可
        auto task = std::make_shared<std::packaged_task<RETURN_TYPE()>>(std::bind(async_func, args...));
        auto future = task->get_future();
        quest_type[func_id] = task;
        return future;
    }


private:
    std::unordered_map<std::string, std::any> quest_type;
};

#endif // !QUEST_TYPE_H