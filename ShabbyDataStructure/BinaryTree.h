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
	/// <typeparam name="T">节点的数据类型</typeparam>
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
		 * 有两个方向建树
		 * 1. 给定固定节点后，对固定节点的关系进行静态构建
		 * 2. 在增添节点的过程中，对已有的结构进行动态构建
		 */
		
		/// <summary>
		/// 静态构建
		/// </summary>
		virtual void build()
		{

		}
		/// <summary>
		/// 动态构建
		/// </summary>
		/// <param name="node">加入的新节点</param>
		virtual void build(std::unique_ptr<TopoNode<T, 1, 2> > node)
		{

		}
	private:
		std::unique_ptr<TopoNode<T, 1, 2> > root;
		std::vector<TopoNode<T, 1, 2> > whole_tree; // 支持树的所有节点可以被线性数据结构存储
	};
}
#endif // !Binary_Tree_H
