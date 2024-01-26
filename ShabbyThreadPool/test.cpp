#include "ShabbyThreadPool.h"
#include <string>
#include <iostream>

int add(int a, int b)
{
    return a + b;
}
int main()
{
    QuestType func_set;
    auto future = func_set.SetAsyncFunctor<int, int, int>("AsyncADD", std::function<int(int, int)>(add), 3, 4);

    ShabbyThreadPool* p_pool = ShabbyThreadPool::GetPoolInstance();
    p_pool->ResizePool(3);
    p_pool->LaunchPool();
    p_pool->AddQuestToPool<int, int, int>("AsyncADD", func_set);

    std::cout << future.get() << std::endl;

    //p_pool->StopPool();
}
