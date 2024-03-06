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
	/// <typeparam name="T">�ڵ����������</typeparam>
	template<typename T>
	class BinaryTree
	{
	public:
		BinaryTree(T data)
		{
			root = std::make_unique<TopoNode<T, 1, 2> >(false);
			root->get()->SetData(data);
			whole_tree.emplace_back(root);
		}
		virtual ~BinaryTree() {}

		/*
		 * ������������
		 * 1. �����̶��ڵ�󣬶Թ̶��ڵ�Ĺ�ϵ���о�̬����
		 * 2. ������ڵ�Ĺ����У������еĽṹ���ж�̬����
		 */
		
		/// <summary>
		/// ��̬����
		/// </summary>
		virtual void build()
		{

		}
		/// <summary>
		/// ��̬����
		/// </summary>
		/// <param name="node">������½ڵ�</param>
		virtual void build(std::unique_ptr<TopoNode<T, 1, 2> > node)
		{

		}
	private:
		std::unique_ptr<TopoNode<T, 1, 2> > root;
		std::vector<TopoNode<T, 1, 2> > whole_tree; // ֧���������нڵ���Ա��������ݽṹ�洢
	};
}
#endif // !Binary_Tree_H
