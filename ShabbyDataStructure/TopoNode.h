#pragma once
#ifndef TOPO_NODE_H
#define TOPO_NODE_H

#include <vector>
#include <memory>
/// <summary>
/// 本类是用于定义所有拓扑数据结构（树、图）的抽象节点类
/// 主要抽象出节点的出度与入度属性
/// </summary>
namespace shabby
{
	template<typename T, size_t IN_DEGREE, size_t OUT_DEGREE>
	class TopoNode
	{
	public:
		/// <summary>
		/// 如果不确定该用哪一种指向性结构，就都创建吧
		/// </summary>
		TopoNode()
		{
			shptr_in.resize(IN_DEGREE);
			unique_in.resize(IN_DEGREE);
			shptr_out.resize(OUT_DEGREE);
			unique_out.resize(OUT_DEGREE);
		}
		/// <summary>
		/// 如果你确定结构的重边性质就可以手动指定
		/// </summary>
		/// <param name="multi_edge">是否重边</param>
		TopoNode(bool multi_edge)
		{
			if (multi_edge)
			{
				shptr_in.resize(IN_DEGREE);
				shptr_out.resize(OUT_DEGREE);
			}
			else
			{
				unique_in.resize(IN_DEGREE);
				unique_out.resize(OUT_DEGREE);
			}
		}
		/// <summary>
		/// 获取多边有向结构的第i个子节点
		/// </summary>
		/// <param name="i">索引i</param>
		/// <returns>如果第i个位置有节点，则返回对应的节点，否则按照vector的at函数报错</returns>
		virtual std::shared_ptr<TopoNode<T> > GetTheIthChildNode(size_t i)
		{
			return shptr_out.at(i);
		}
		/// <summary>
		/// 获取单边有向结构的第i个子节点
		/// </summary>
		/// <param name="i">索引i</param>
		/// <returns>如果第i个位置有节点，则返回对应的节点，否则按照vector的at函数报错</returns>
		virtual std::unique_ptr<TopoNode<T> > GetTheIthChildNode(size_t i)
		{
			return unique_out.at(i);
		}
		/// <summary>
		/// 获取多边有向结构的第i个父节点
		/// </summary>
		/// <param name="i">索引i</param>
		/// <returns>如果第i个位置有节点，则返回对应的节点，否则按照vector的at函数报错</returns>
		virtual std::shared_ptr<TopoNode<T> > GetTheIthParentNode(size_t i)
		{
			return shptr_in.at(i);
		}
		/// <summary>
		/// 获取单边有向结构的第i个父节点
		/// </summary>
		/// <param name="i">索引i</param>
		/// <returns>如果第i个位置有节点，则返回对应的节点，否则按照vector的at函数报错</returns>
		virtual std::unique_ptr<TopoNode<T> > GetTheIthParentNode(size_t i)
		{
			return unique_in.at(i);
		}
		/// <summary>
		/// 设置第i个单边子节点
		/// </summary>
		/// <param name="out_node">输入的单边图的节点</param>
		/// <param name="i">单边点容器索引</param>
		virtual void SetTheIthChildNode(std::unique_ptr<TopoNode<T> > out_node, size_t i)
		{
			auto noted_ptr = unique_out.at(i);
			noted_ptr(std::move(in_node));
		}
		/// <summary>
		/// 设置第i个重边子节点
		/// </summary>
		/// <param name="out_node">输入的多边图节点</param>
		/// <param name="i">多边点容器索引</param>
		virtual void SetTheIthChildNode(std::shared_ptr<TopoNode<T> > out_node, size_t i)
		{
			auto noted_ptr = shptr_out.at(i);
			noted_ptr(std::move(in_node));
		}
		/// <summary>
		/// 设置第i个单边父节点
		/// </summary>
		/// <param name="in_node">输入的单边图节点</param>
		/// <param name="i">单边点容器索引</param>
		virtual void SetTheIthParentNode(std::unique_ptr<TopoNode<T> > in_node, size_t i)
		{
			auto noted_ptr = unique_in.at(i);
			noted_ptr(std::move(in_node));
		}
		/// <summary>
		/// 设置第i个重边父节点
		/// </summary>
		/// <param name="in_node">输入的重边图节点</param>
		/// <param name="i">重边点容器索引</param>
		virtual void SetTheIthParentNode(std::shared_ptr<TopoNode<T> > in_node, size_t i)
		{
			auto noted_ptr = shptr_in.at(i);
			noted_ptr(std::move(in_node));
		}
		/// <summary>
		/// 返回节点值引用
		/// </summary>
		/// <returns>节点类型引用</returns>
		virtual T& GetData()
		{
			return Data;
		}
		/// <summary>
		/// 默认T类型使用=号来执行赋值操作，否则应该覆盖
		/// </summary>
		/// <param name="data">输入的数据值</param>
		virtual void SetData(const T& data)
		{
			Data = data;
		}
		virtual ~TopoNode() {}
	private:
		T Data;
		std::vector<std::shared_ptr<TopoNode<T> > > shptr_in, shptr_out; // 使用共享指针可以实现有向重边指向结构
		std::vector<std::unique_ptr<TopoNode<T> > > unique_in, unique_out; // 使用独享指针可以实现有向无重边指向结构，且效率比shared_ptr高
	};
}

#endif // !TOPO_NODE_H
