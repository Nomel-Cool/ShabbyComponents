#include <iostream>
#include <future>
#include "ShabbyThreadPool.h"

void exampleTask(int n) {
    std::cout << "Task executed with value: " << n << " by thread " << std::this_thread::get_id() << std::endl;
}

int main() {
    // 获取线程池实例
    ShabbyThreadPool& pool = ShabbyThreadPool::GetInstance();

    // 提交一些任务
    auto future1 = pool.Enqueue(exampleTask, 10);
    auto future2 = pool.Enqueue(exampleTask, 20);
    auto future3 = pool.Enqueue([](int x) { std::cout << "Lambda task with value: " << x << " by thread " << std::this_thread::get_id() << std::endl; }, 30);

    // 等待任务完成
    future1.get();
    future2.get();
    future3.get();

    std::cout << "All tasks have been executed." << std::endl;

    return 0;
}
