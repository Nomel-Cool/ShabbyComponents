#pragma once
#ifndef SORTING_H
#define SORTING_H
#include <algorithm>
#include <vector>
namespace shabby
{
	class Sorting
	{
	public:
		// һ����������Դ��һ��������

	protected:

	private:

	};
	/// <summary>
	/// ����
	/// </summary>
	template<typename T>
	class FastSort : public Sorting
	{
	public:
		/// <summary>
		/// Ĭ����������
		/// </summary>
		/// <param name="source">��������Դ</param>
		/// <param name="direction">������trueΪ���򣬷�����</param>
		FastSort(std::vector<T>& source, bool direction)
		{
			if (!direction)
				std::sort(source.begin(), source.end(), cmp);
			else
				std::sort(source.begin(), source.end());
		}
		/// <summary>
		/// �����أ����붨����
		/// </summary>
		/// <param name="">��������Դ</param>
		/// <param name="direction">������trueΪ���򣬷�����</param>
		Fast(T[] source, bool direction)
		{
			int n = sizeof(source) / sizeof(source[0]);
			if (!direction)
				std::sort(source, source + n, cmp);
			else
				std::sort(source, source + n);
		}
		virtual ~FastSort() {}
		virtual bool cmp(const T& a, const T& b)
		{
			return a > b;
		}
	};
}
#endif // !SORTING_H
