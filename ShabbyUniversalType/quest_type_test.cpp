#include "QuestType.h"

int main()
{
	QuestType func_set;
	func_set.SetFunctor<void, int, int>("ADD", std::function<void(int, int)>([=](int a, int b) { printf((std::to_string(a + b) + "\n").c_str()); }));
	std::any any_add = func_set.GetFunctor("ADD");
	auto f = func_set.CastAnyToFunction<void,int,int>(any_add);
	f(1, 2);
	return 0;
}