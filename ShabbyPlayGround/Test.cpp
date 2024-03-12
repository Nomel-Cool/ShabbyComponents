#include <iostream>
#include <memory>

/// <summary>
/// ���Զ���:std::make_shared
/// ����Ŀ��:make_shared�Ƿ��ʹ�����ü�������
/// ���Բ���:
/// 1.����һ��sharedָ��P1ָ��int���ͶѶ��󣬴�ӡ���ü���
/// ���̲��Խ��:1
/// 2.ͨ��std::make_shared<int>(*P1)�ٴ���һ��sharedָ��P2�����ǲ�ʹ����ֵ���ܣ���ӡ���ü���
/// ���̲��Խ��:1
/// 3.�Ե�2��ʹ����ֵ���ܣ���ӡ���ü���
/// ���̲��Խ��:1
/// 4.��ӡ��3����ֵP2�����ü���
/// ���̲��Խ��:1
/// 5.ʹ�ø�ֵ=��P3���ݸ�P4
/// ���̴�ӡ���:2
/// 6.ʹ�����ý�P4���ݸ�P5
/// ���̴�ӡ���:2
/// ���ϴ�ӡ�Ĳ��Խ������1��˵����û�ж��ָ��ͬʱָ��ͬһ���ڴ棬ԭ�������make_shared��ʵ���Ǵ�����һ���µ�int���󣬵ȼ���new int(___)
/// ������һ��������ʼ���������Ҫ�������ü���������Ҫ��һ��shared_ptr��ֵ����һ��shared_ptr�ϼ��ɣ���ʹ�����ò��С�
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
/// ���������������ָ�����ʵ��
/// </summary>
/// <returns></returns>
int main()
{
	test4_MAKE_SHARED();
}