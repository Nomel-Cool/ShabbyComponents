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
		TreeNode(T data):Data(data) {}
		virtual ~TreeNode() {}
		virtual TreeNode* GetLeftChild()
		{
			return left_node;
		}
		virtual TreeNode* GetRightChild()
		{
			return right_node;
		}
		virtual TreeNode* GetParentNode()
		{
			return parent_node;
		}
		virtual void SetLeftChild(TreeNode* left_child)
		{
			left_node = left_child;
		}
		virtual void SetRightChild(TreeNode* right_child)
		{
			right_node = right_child;
		}
		virtual void SetParentNode(TreeNode* parent)
		{
			parent_node = parent;
		}
		virtual T GetData()
		{
			return Data;
		}
	protected:
		TreeNode* left_node = nullptr, * right_node = nullptr, * parent_node = this;
		T Data;
	private:
	};

	template<typename T>
	class BinaryTree
	{
	public:
		/// <summary>
		/// 插入设置好子关系以及父关系的节点来建立
		/// </summary>
		/// <param name="input_node"></param>
		virtual void BuildByAdding(TreeNode<T>& input_node)
		{
			tree.emplace_back(input_node);
		}
		virtual void BuildTree() {};
		/// <summary>
		/// 设置根节点
		/// </summary>
		/// <param name="node">T类型表示的根节点</param>
		virtual void SetRootNode(T node)
		{
			root = &node;
		}
		/// <summary>
		/// 非递归版本的前序遍历
		/// </summary>
		/// <returns>前序遍历序列</returns>
		virtual std::vector<T> preorderTraversal(void(*)(T) interfere_action)
		{
			std::stack<TreeNode<T>*> sk;
			std::vector<T> v;
			if (root == nullptr)return v;//如果连根都没有，就直接返回空序列
			sk.push(root);//栈的初始化
			TreeNode<T>* next;
			while (!sk.empty())//否则必定起码有一个根节点作为初始节点
			{
				next = sk.top();//用临时变量保存弹出的元素
				v.push_back(next->GetData());//把弹出元素的内容保存到vector上

				interfere_action(next->GetData()); // 入栈时干预

				sk.pop();//真正弹出该元素
				action_first(next);
				if (next->right != nullptr)//先压右子树
					sk.push(next->right);
				if (next->left != nullptr)//再压左子树
					sk.push(next->left);
			}
			return v;
		}

		virtual void IndorderTraversal(TreeNode<T>& start_node)
		{

		}
		virtual void PostorderTraversal(TreeNode<T>& start_node)
		{

		}
	private:
		TreeNode<T>* root;
		std::vector<TreeNode> tree;
	};
}
#endif // !Binary_Tree_H
