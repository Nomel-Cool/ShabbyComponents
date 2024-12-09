#include "EventsQueue.h"

template<typename T>
bool TaskModelQueue<T>::Empty() const
{
	std::lock_guard<std::mutex> lock(mutex_);
	return inner_queue.empty();
}

template<typename T>
void TaskModelQueue<T>::AddQuestToQueue(std::unique_ptr<T> enlisted_node)
{
	std::lock_guard<std::mutex> lock(mutex_);
	inner_queue.push(std::move(enlisted_node));
	cond_.notify_one();
}

template<typename T>
std::unique_ptr<T> TaskModelQueue<T>::GetQuestFromQueue()
{
	std::unique_lock<std::mutex> lock(mutex_);
	cond_.wait(lock, [this] { return !inner_queue.empty(); });
	auto consumed_node = std::move(inner_queue.front());
	inner_queue.pop();
	return consumed_node;
}