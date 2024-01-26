#include "ShabbyThreadPool.h"
#include <string>
#include <iostream>

QuestType add(int a, int b)
{
    std::cout << "Calculate Result: " << a + b << std::endl;
    QuestType tmp;
    return tmp;
}
int main()
{
    QuestType func_set;
    auto future1 = func_set.SetAsyncFunctor<QuestType, int, int>("AsyncADD1", std::function<QuestType(int, int)>(add), 1, 2);
    auto future2 = func_set.SetAsyncFunctor<QuestType, int, int>("AsyncADD2", std::function<QuestType(int, int)>(add), 3, 4);
    auto future3 = func_set.SetAsyncFunctor<QuestType, int, int>("AsyncADD3", std::function<QuestType(int, int)>(add), 5, 6);
    auto future4 = func_set.SetAsyncFunctor<QuestType, int, int>("AsyncADD4", std::function<QuestType(int, int)>(add), 7, 8);

    ShabbyThreadPool* p_pool = ShabbyThreadPool::GetPoolInstance();
    p_pool->ResizePool(3);
    p_pool->LaunchPool();
    p_pool->AddQuestToPool<QuestType, int, int>("AsyncADD1", func_set);
    p_pool->AddQuestToPool<QuestType, int, int>("AsyncADD2", func_set);
    p_pool->AddQuestToPool<QuestType, int, int>("AsyncADD3", func_set);
    p_pool->AddQuestToPool<QuestType, int, int>("AsyncADD4", func_set);

    future1.get();
    future2.get();
    future3.get();
    future4.get();

    //p_pool->StopPool();
}
