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
    auto future = func_set.SetAsyncFunctor<QuestType, int, int>("AsyncADD", std::function<QuestType(int, int)>(add), 3, 4);

    ShabbyThreadPool* p_pool = ShabbyThreadPool::GetPoolInstance();
    p_pool->ResizePool(3);
    p_pool->LaunchPool();
    p_pool->AddQuestToPool<QuestType, int, int>("AsyncADD", func_set);

    future.get();

    //p_pool->StopPool();
}
