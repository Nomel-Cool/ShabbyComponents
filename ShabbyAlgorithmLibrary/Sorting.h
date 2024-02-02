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
		// 一个待排序资源，一个排序方向

	protected:

	private:

	};
	/// <summary>
	/// 快排
	/// </summary>
	template<typename T>
	class FastSort : public Sorting
	{
	public:
		/// <summary>
		/// 默认升序排序
		/// </summary>
		/// <param name="source">待排序资源</param>
		/// <param name="direction">排序方向true为正序，否则倒序</param>
		FastSort(std::vector<T>& source, bool direction)
		{
			if (!direction)
				std::sort(source.begin(), source.end(), cmp);
			else
				std::sort(source.begin(), source.end());
		}
		/// <summary>
		/// （重载）传入定数组
		/// </summary>
		/// <param name="">待排序资源</param>
		/// <param name="direction">排序方向true为正序，否则倒序</param>
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
