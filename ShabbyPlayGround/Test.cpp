#include <iostream>
#include <memory>

/// <summary>
/// ���Զ���:std::make_shared
/// ����Ŀ��:make_shared�Ƿ��ʹ�����ü�������
/// ���Բ���:
/// 1.����һ��int�Ѷ��� A
/// 2.����һ��sharedָ��P1 ��A �����乹�캯��
/// 3.ͨ��std::make_shared<int>(A)�ٴ���һ��sharedָ��P2
/// ���Լ���:�鿴P �����ü����Ƿ�Ϊ2
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
/// ���������������ָ�����ʵ��
/// </summary>
/// <returns></returns>
int main()
{

}