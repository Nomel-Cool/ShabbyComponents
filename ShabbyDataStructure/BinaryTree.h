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
		virtual void BuildByAdding(TopoNode<T>* input_node)
		{
			m_tree.push_back(input_node);
		}
		/// <summary>
		/// �ͷŹ������������ж�ָ̬���ڴ�
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
		/// ���ø��ڵ�
		/// </summary>
		/// <param name="node">T���ͱ�ʾ�ĸ��ڵ�</param>
		virtual void SetRootNode(TopoNode<T>* node)
		{
			m_root = node;
		}
		/// <summary>
		/// �ǵݹ�汾��ǰ�����
		/// </summary>
		/// <returns>ǰ���������</returns>
		virtual std::vector<T> preorderTraversal()
		{
			std::stack<TopoNode<T>*> sk;
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
