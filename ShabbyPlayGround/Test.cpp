#include <iostream>
#include <memory>

/// <summary>
/// 测试对象:std::make_shared
/// 测试目的:make_shared是否会使得引用计数增加
/// 测试步骤:
/// 1.创建一个shared指针P1指向int类型堆对象，打印引用计数
/// 过程测试结果:1
/// 2.通过std::make_shared<int>(*P1)再创建一个shared指针P2，但是不使用左值接受，打印引用计数
/// 过程测试结果:1
/// 3.对第2点使用左值接受，打印引用计数
/// 过程测试结果:1
/// 4.打印第3点左值P2的引用计数
/// 过程测试结果:1
/// 5.使用赋值=将P3传递给P4
/// 过程打印结果:2
/// 6.使用引用将P4传递给P5
/// 过程打印结果:2
/// 综上打印的测试结果都是1，说明并没有多个指针同时指向同一个内存，原因就在于make_shared事实上是创建了一个新的int对象，等价于new int(___)
/// 所以它一般用来初始化，如果想要增加引用计数，则需要令一个shared_ptr赋值到另一个shared_ptr上即可，但使用引用不行。
/// </summary>
void test4_MAKE_SHARED()
{
	std::shared_ptr<int> P1(new int(5));
	std::cout << "Use_Count_1: " << P1.use_count() << std::endl;
	std::make_shared<int>(*P1);
	std::cout << "Use_Count_2: " << P1.use_count() << std::endl;
	auto P2 = std::make_shared<int>(*P1);
	std::cout << "Use_Count_3: " << P2.use_count() << std::endl;
	auto P3 = std::make_shared<int>(*(P1.get()));
	std::cout << "Use_Count_4: " << P3.use_count() << std::endl;
	auto P4 = P3;
	std::cout << "Use_Count_5: " << P4.use_count() << std::endl;
	auto& P5 = P4;
	std::cout << "Use_Count_6: " << P5.use_count() << std::endl;
}

/// <summary>
/// 本函数用于做各种各样的实验
/// </summary>
/// <returns></returns>
int main()
{
	test4_MAKE_SHARED();
}