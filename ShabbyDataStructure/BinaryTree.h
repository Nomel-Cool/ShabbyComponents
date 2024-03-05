#pragma once
#ifndef Binary_Tree_H
#define Binary_Tree_H

#include <stack>
#include <vector>

#include "../ShabbyUniversalType/QuestType.h"
#include "TopoNode.h"
using namespace shabby;
namespace shabby
{
	/// <summary>
	/// 为了管理树形结构的指针链接，保证能够有效地删除，树的所有指针都应该被保存，不使用该结构体后，应该统一删除
	/// </summary>
	/// <typeparam name="T"></typeparam>
	template<typename T>
	class BinaryTree
	{
	public:
		BinaryTree() {}
		~BinaryTree() {}
		/// <summary>
		/// 插入设置好子关系以及父关系的节点来建立
		/// </summary>
		/// <param name="input_node"></param>
		virtual void BuildByAdding(TopoNode<T>* input_node)
		{
			m_tree.push_back(input_node);
		}
		/// <summary>
		/// 释放哈夫曼树的所有动态指针内存
		/// </summary>
		virtual void FreeTree()
		{
			for (TopoNode<T>* node : m_tree)
			{
				delete node->GetLeftChild();
				delete node->GetRightChild();
				delete node->GetParentNode();
				delete node;
			}
		}
		/// <summary>
		/// 设置根节点
		/// </summary>
		/// <param name="node">T类型表示的根节点</param>
		virtual void SetRootNode(TopoNode<T>* node)
		{
			m_root = node;
		}
		/// <summary>
		/// 非递归版本的前序遍历
		/// </summary>
		/// <returns>前序遍历序列</returns>
		virtual std::vector<T> preorderTraversal()
		{
			std::stack<TopoNode<T>*> sk;
			std::vector<T> v;
			sk.push(m_root);//栈的初始化
			while (!sk.empty())//否则必定起码有一个根节点作为初始节点
			{
				auto next = sk.top();//用临时变量保存弹出的元素
				v.push_back(next->GetData());//把弹出元素的内容保存到vector上
				sk.pop();//真正弹出该元素
				if (next->GetRightChild() != nullptr)//先压右子树
					sk.push(next->GetRightChild());
				if (next->GetLeftChild() != nullptr)//再压左子树
					sk.push(next->GetLeftChild());
			}
			return v;
		}
		virtual void IndorderTraversal(TopoNode<T>& start_node)
		{

		}
		virtual void PostorderTraversal(TopoNode<T>& start_node)
		{

		}

		TopoNode<T>* m_root = nullptr;
		std::vector<TopoNode<T, 1, 2> > m_tree;
	private:
	};

	/// <summary>
	/// 最大堆
	/// </summary>
	template<typename T>
	class GreaterHeap
	{
	public:

	private:

	};

	/// <summary>
	/// 最小堆
	/// </summary>
	template<typename T>
	class LessHeap
	{
	public:

	private:

	};
}
#endif // !Binary_Tree_H
