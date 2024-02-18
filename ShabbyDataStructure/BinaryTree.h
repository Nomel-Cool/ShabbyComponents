#pragma once
#ifndef Binary_Tree_H
#define Binary_Tree_H

#include <stack>
#include <vector>

#include "../ShabbyUniversalType/QuestType.h"
using namespace shabby;
namespace shabby
{
	/// <summary>
	/// 本工程志向收集所有的二叉树，包括但不限于AVL，红黑，B+，B-
	/// </summary>
	template<typename T>
	class TreeNode
	{
	public:
		TreeNode() {}
		virtual ~TreeNode() {}
		bool operator>(const TreeNode<T>& _other) const
		{
			return Data > _other.Data;
		}
		virtual TreeNode<T>* GetLeftChild() // Wtf，返回非模板的TreeNode也是能编译过去的？
		{
			return left_node;
		}  
		virtual TreeNode<T>* GetRightChild()
		{
			return right_node;
		}
		virtual TreeNode<T>* GetParentNode()
		{
			return parent_node;
		}
		virtual void SetLeftChild(TreeNode<T>* left_child)
		{
			left_node = left_child;
		}
		virtual void SetRightChild(TreeNode<T>* right_child)
		{
			right_node = right_child;
		}
		virtual void SetParentNode(TreeNode<T>* parent)
		{
			parent_node = parent;
		}
		virtual void SetData(T data)
		{
			Data = data;
		}
		virtual T& GetData()
		{
			return Data;
		}
	protected:
		TreeNode<T>* left_node = nullptr, * right_node = nullptr, * parent_node = nullptr;
		T Data;
	private:
	};

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
		virtual void BuildByAdding(TreeNode<T>* input_node)
		{
			m_tree.push_back(input_node);
		}

		/// <summary>
		/// 释放哈夫曼树的所有动态指针内存
		/// </summary>
		virtual void FreeTree()
		{
			for (TreeNode<T>* node : m_tree)
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
		virtual void SetRootNode(TreeNode<T>* node)
		{
			m_root = node;
		}
		/// <summary>
		/// 非递归版本的前序遍历
		/// </summary>
		/// <returns>前序遍历序列</returns>
		virtual std::vector<T> preorderTraversal()
		{
			std::stack<TreeNode<T>*> sk;
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
		virtual void IndorderTraversal(TreeNode<T>& start_node)
		{

		}
		virtual void PostorderTraversal(TreeNode<T>& start_node)
		{

		}

		TreeNode<T>* m_root = nullptr;
		std::vector<TreeNode<T>*> m_tree;
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
