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
	/// ������־���ռ����еĶ�������������������AVL����ڣ�B+��B-
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
		/// �������ú��ӹ�ϵ�Լ�����ϵ�Ľڵ�������
		/// </summary>
		/// <param name="input_node"></param>
		virtual void BuildByAdding(TreeNode<T>& input_node)
		{
			tree.emplace_back(input_node);
		}
		virtual void BuildTree() {};
		/// <summary>
		/// ���ø��ڵ�
		/// </summary>
		/// <param name="node">T���ͱ�ʾ�ĸ��ڵ�</param>
		virtual void SetRootNode(T node)
		{
			root = &node;
		}
		/// <summary>
		/// �ǵݹ�汾��ǰ�����
		/// </summary>
		/// <returns>ǰ���������</returns>
		virtual std::vector<T> preorderTraversal(void(*)(T) interfere_action)
		{
			std::stack<TreeNode<T>*> sk;
			std::vector<T> v;
			if (root == nullptr)return v;//���������û�У���ֱ�ӷ��ؿ�����
			sk.push(root);//ջ�ĳ�ʼ��
			TreeNode<T>* next;
			while (!sk.empty())//����ض�������һ�����ڵ���Ϊ��ʼ�ڵ�
			{
				next = sk.top();//����ʱ�������浯����Ԫ��
				v.push_back(next->GetData());//�ѵ���Ԫ�ص����ݱ��浽vector��

				interfere_action(next->GetData()); // ��ջʱ��Ԥ

				sk.pop();//����������Ԫ��
				action_first(next);
				if (next->right != nullptr)//��ѹ������
					sk.push(next->right);
				if (next->left != nullptr)//��ѹ������
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
