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
	/// 定义二叉树先定义对应的节点
	/// </summary>
	template<typename T>
	class BinaryNode : public TopoNode<T, 1, 2>
	{
	public:
		virtual std::unique_ptr<TopoNode<T, 1, 2> > GetLeftChildUnique()
		{
			return GetTheIthChildNodeUnique(0);
		}
		virtual std::unique_ptr<TopoNode<T, 1, 2> > GetRightChildUnique()
		{
			return GetTheIthChildNodeUnique(1);
		}
		virtual std::unique_ptr<TopoNode<T, 1, 2> > GetTheParentUnique()
		{
			return GetTheIthParentNodeUnique(0);
		}

		virtual std::shared_ptr<TopoNode<T, 1, 2> > GetLeftChildShared()
		{
			return GetTheIthChildNodeShared(0);
		}
		virtual std::shared_ptr<TopoNode<T, 1, 2> > GetRightChildShared()
		{
			return GetTheIthChildNodeShared(1);
		}
		virtual std::shared_ptr<TopoNode<T, 1, 2> > GetTheParentShared()
		{
			return GetTheIthParentNodeShared(0);
		}

		virtual void SetLeftChild(std::unique_ptr<TopoNode<T, 1, 2> >& node)
		{
			if (node->get() == nullptr)
				return;
			SetTheIthChildNode(node, 0);
		}
		virtual void SetRightChild(std::unique_ptr<TopoNode<T, 1, 2> >& node)
		{
			if (node->get() == nullptr)
				return;
			SetTheIthChildNode(node, 1);
		}
		virtual void SetTheParent(std::unique_ptr<TopoNode<T, 1, 2> >& node)
		{
			if (node->get() == nullptr)
				return;
			SetTheIthParentNode(node, 0);
		}
		virtual void SetLeftChild(std::shared_ptr<TopoNode<T, 1, 2> > node)
		{
			if (node->get() == nullptr)
				return;
			SetTheIthChildNode(node, 0);
		}
		virtual void SetRightChild(std::shared_ptr<TopoNode<T, 1, 2> > node)
		{
			if (node->get() == nullptr)
				return;
			SetTheIthChildNode(node, 1);
		}
		virtual void SetTheParent(std::shared_ptr<TopoNode<T, 1, 2> > node)
		{
			if (node->get() == nullptr)
				return;
			SetTheIthParentNode(node, 0);
		}

	private:
	};

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
			root = std::make_unique<BinaryNode>(false);
			root->get()->SetData(data);

			whole_tree.emplace_back(root);
		}
		virtual ~BinaryTree() {}

		/// <summary>
		/// 单边动态构建
		/// </summary>
		/// <param name="current_mod_node">正在修改的节点</param>
		/// <param name="p_node">加入的新节点</param>
		virtual bool build(std::unique_ptr<BinaryNode>& current_mod_node, const BinaryNode& node) // 啰嗦一嘴，如果传参是智能指针，必须用引用才能修改其内存
		{
			if (p_node->get() == nullptr)
				return false;
			auto find_result = std::find_if(whole_tree.begin(), whole_tree.end(), &current_mod_node{ // 由于使用了find_if，所以复杂度为1+2+...+N = O(N²)
				return node.get() == current_mod_node.get();
			});
			if (find_result == whole_tree.end())
				return false;
			current_mod_node = std::make_unique<BinaryNode>(node);
			return true;
		}

		/// <summary>
		/// 多边动态构建
		/// </summary>
		/// <param name="current_mod_node">正在修改的节点</param>
		/// <param name="p_node">加入的新节点</param>
		/// <returns></returns>
		virtual bool build(std::shared_ptr<BinaryNode>& current_mod_node, BinaryNode node)
		{
			if (p_node->get() == nullptr)
				return false;
			auto find_result = std::find_if(whole_tree.begin(), whole_tree.end(), &current_mod_node{ // 由于使用了find_if，所以复杂度为1+2+...+N = O(N²)
				return node.get() == current_mod_node.get();
				});
			if (find_result == whole_tree.end())
				return false;
			current_mod_node = std::make_shared<BinaryNode>(node);
			return true;
		}
	private:
		std::unique_ptr<BinaryNode> root;
		std::vector<std::unique_ptr<BinaryNode> > whole_tree; // 支持树的所有节点可以被线性数据结构存储
	};
}
#endif // !Binary_Tree_H
