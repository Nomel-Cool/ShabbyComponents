#include <iostream>
#include <memory>

/// <summary>
/// 测试对象:std::make_shared
/// 测试目的:make_shared是否会使得引用计数增加
/// 测试步骤:
/// 1.创建一个int堆对象 A
/// 2.创建一个shared指针P1 将A 传入其构造函数
/// 3.通过std::make_shared<int>(A)再创建一个shared指针P2
/// 测试检验:查看P 的引用计数是否为2
/// </summary>
void test4_MAKE_SHARED()
{
	int* A = new int(5);
	std::shared_ptr<int> P1(A);
	std::cout << "Use_Count_1: " << P1.use_count() << std::endl;
	std::shared_ptr<int> P2 = std::make_shared<int>(A);
	std::cout << "Use_Count_2: " << P2.use_count() << std::endl;
}

/// <summary>
/// 本函数用于做各种各样的实验
/// </summary>
/// <returns></returns>
int main()
{

}