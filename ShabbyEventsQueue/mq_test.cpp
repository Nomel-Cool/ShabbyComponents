#include "EventsQueue.h"
#include <thread>


EventsQueue<QuestType> eq;

void AddQ()
{
	int i = 0;
	while (1)
	{
		printf("AddQ\n");
		QuestType* func_set = new QuestType();
		func_set->SetFunctor<void, int, int>("ADD", std::function<void(int, int)>([=](int a, int b) { printf((std::to_string(a * i + b) + "\n").c_str()); }));
		eq.AddQuestToQueue(*func_set);
		if (i++ == 10)
			break;
	}
}

void RemoveQ()
{
	int i = 0, j = 1;
	while (1)
	{
		printf("RemoveQ\n");
		auto func = eq.GetQuestFromQueue();
		func->GetFunctor<void, int, int>("ADD")(i, j);
		i++;
		j *= 2;
		if (i == 10)
			break;
	}
}

int main()
{

	std::thread add_quest(AddQ);
	std::thread get_quest(RemoveQ);

	add_quest.join();
	get_quest.join();

	return 0;
}