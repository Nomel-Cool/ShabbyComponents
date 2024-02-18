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
		TreeNode() {}
		virtual ~TreeNode() {}
		bool operator>(const TreeNode<T>& _other) const
		{
			return Data > _other.Data;
		}
		virtual TreeNode<T>* GetLeftChild() // Wtf�����ط�ģ���TreeNodeҲ���ܱ����ȥ�ģ�
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
	/// Ϊ�˹������νṹ��ָ�����ӣ���֤�ܹ���Ч��ɾ������������ָ�붼Ӧ�ñ����棬��ʹ�øýṹ���Ӧ��ͳһɾ��
	/// </summary>
	/// <typeparam name="T"></typeparam>
	template<typename T>
	class BinaryTree
	{
	public:
		BinaryTree() {}
		~BinaryTree() {}
		/// <summary>
		/// �������ú��ӹ�ϵ�Լ�����ϵ�Ľڵ�������
		/// </summary>
		/// <param name="input_node"></param>
		virtual void BuildByAdding(TreeNode<T>* input_node)
		{
			m_tree.push_back(input_node);
		}

		/// <summary>
		/// �ͷŹ������������ж�ָ̬���ڴ�
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
		/// ���ø��ڵ�
		/// </summary>
		/// <param name="node">T���ͱ�ʾ�ĸ��ڵ�</param>
		virtual void SetRootNode(TreeNode<T>* node)
		{
			m_root = node;
		}
		/// <summary>
		/// �ǵݹ�汾��ǰ�����
		/// </summary>
		/// <returns>ǰ���������</returns>
		virtual std::vector<T> preorderTraversal()
		{
			std::stack<TreeNode<T>*> sk;
			std::vector<T> v;
			sk.push(m_root);//ջ�ĳ�ʼ��
			while (!sk.empty())//����ض�������һ�����ڵ���Ϊ��ʼ�ڵ�
			{
				auto next = sk.top();//����ʱ�������浯����Ԫ��
				v.push_back(next->GetData());//�ѵ���Ԫ�ص����ݱ��浽vector��
				sk.pop();//����������Ԫ��
				if (next->GetRightChild() != nullptr)//��ѹ������
					sk.push(next->GetRightChild());
				if (next->GetLeftChild() != nullptr)//��ѹ������
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
	/// ����
	/// </summary>
	template<typename T>
	class GreaterHeap
	{
	public:

	private:

	};

	/// <summary>
	/// ��С��
	/// </summary>
	template<typename T>
	class LessHeap
	{
	public:

	private:

	};
}
#endif // !Binary_Tree_H
