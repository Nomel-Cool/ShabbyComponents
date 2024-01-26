#include <iostream>

#include "QuestType.h"

int add(int a, int b)
{
	return a + b;
}

int main()
{
#pragma region Õ¨≤ΩNode≤‚ ‘
	QuestType func_set;
	func_set.SetFunctor<void, int, int>("ADD", std::function<void(int, int)>([=](int a, int b) { printf((std::to_string(a + b) + "\n").c_str()); }));
	std::any any_add = func_set.GetFunctor("ADD");
	auto f = func_set.CastAnyToFunction<void,int,int>(any_add);
	f(1, 2);
#pragma endregion

#pragma region —”≥ŸNode≤‚ ‘
	auto future = func_set.SetAsyncFunctor<int, int, int>("AsyncADD", std::function<int(int, int)>(add), 3, 4);
	auto any_pm = func_set.GetAsyncFunctor("AsyncADD");
	auto pm = func_set.CastAnyToPromise<int>(any_pm);
	(*pm)();
	std::cout << future.get() << std::endl;
#pragma endregion
	return 0;
}