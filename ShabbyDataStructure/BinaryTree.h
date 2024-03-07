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
		BinaryNode() : TopoNode<T, 1, 2>()
		{

		}
		BinaryNode(bool duplicate_edge) : TopoNode<T, 1, 2>(duplicate_edge), edge_type(duplicate_edge)
		{

		}
		/* 啰嗦一嘴：由于RVO机制，GetTheParent并不会使得返回的指针发生拷贝从而违反Uniqueptr唯一性或者引起Sharedptr引用计数增加 */
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

		virtual void SetLeftChild(std::shared_ptr<TopoNode<T, 1, 2> >& node)
		{
			if (node->get() == nullptr)
				return;
			SetTheIthChildNode(node, 0);
		}
		virtual void SetRightChild(std::shared_ptr<TopoNode<T, 1, 2> >& node)
		{
			if (node->get() == nullptr)
				return;
			SetTheIthChildNode(node, 1);
		}
		virtual void SetTheParent(std::shared_ptr<TopoNode<T, 1, 2> >& node)
		{
			if (node->get() == nullptr)
				return;
			SetTheIthParentNode(node, 0);
		}

		bool GetEdgeType()
		{
			return edge_type;
		}
	private:
		bool edge_type;
	};

	/// <summary>
	/// 为了管理树形结构的指针链接，保证能够有效地删除，树的所有指针都应该被保存，不使用该结构体后，应该统一删除
	/// </summary>
	/// <typeparam name="T">节点的数据类型</typeparam>
	template<typename T>
	class BinaryTree
	{
	public:
		BinaryTree(BinaryNode data, bool duplicate_edge) :edge_type(duplicate_edge)
		{
			if (duplicate_edge)
			{
				sroot = std::make_shared<BinaryNode>(data, duplicate_edge);
				swhole_tree.emplace_back(sroot);
			}
			else
			{
				uroot = std::make_unique<BinaryNode>(data, duplicate_edge);
				uwhole_tree.emplace_back(uroot);
			}
		}
		virtual ~BinaryTree() {}
		
		/// <summary>
		/// 单边动态构建
		/// 由于初始化时用户总是持有一个根节点，所以用户执行建树后始终可以持有一个有效的树容器，用于持续获取节点
		/// </summary>
		/// <param name="current_mod_node">正在修改的节点</param>
		/// <param name="node">加入的新节点</param>
		/// <returns>返回当前建好的树，如果加入的节点的边属性不匹配则返回空容器</returns>
		virtual std::vector<std::unique_ptr<BinaryNode> >& insert(std::unique_ptr<BinaryNode>& current_mod_node, const BinaryNode& node) // 啰嗦一嘴，如果传参是智能指针，必须用引用才能修改其内存
		{
			if (node.GetEdgeType())
				return std::vector<std::unique_ptr<BinaryNode> >();
			auto find_result = std::find_if(uwhole_tree.begin(), uwhole_tree.end(), &current_mod_node{ // 由于使用了find_if，所以复杂度为1+2+...+N = O(N²)
				return std::vector<std::unique_ptr<BinaryNode> >();
			});
			if (find_result == uwhole_tree.end())
				return std::vector<std::unique_ptr<BinaryNode> >();
			current_mod_node = std::make_unique<BinaryNode>(node);
			return uwhole_tree;
		}

		/// <summary>
		/// 多边动态构建
		/// 由于初始化时用户总是持有一个根节点，所以用户执行建树后始终可以持有一个有效的树容器，用于持续获取节点
		/// </summary>
		/// <param name="current_mod_node">正在修改的节点</param>
		/// <param name="node">加入的新节点</param>
		/// <returns>返回当前建好的树，如果加入的节点的边属性不匹配则返回空容器</returns>
		virtual std::vector<std::shared_ptr<BinaryNode> >& insert(std::shared_ptr<BinaryNode>& current_mod_node, const BinaryNode& node)
		{
			if (!node.GetEdgeType())
				return std::vector<std::shared_ptr<BinaryNode> >();
			auto find_result = std::find_if(swhole_tree.begin(), swhole_tree.end(), &current_mod_node{ // 由于使用了find_if，所以复杂度为1+2+...+N = O(N²)
				return std::vector<std::shared_ptr<BinaryNode> >();
				});
			if (find_result == swhole_tree.end())
				return std::vector<std::shared_ptr<BinaryNode> >();
			current_mod_node = std::make_shared<BinaryNode>(node);
			return swhole_tree;
		}

		/// <summary>
		/// 删除树中指定节点，可以返回删除的局部，用于快速处理删除后的重建任务
		/// </summary>
		/// <param name="current_mod_node">正在修改的节点</param>
		/// <returns>返回当前建好的树</returns>
		virtual std::vector<std::unique_ptr<BinaryNode> >& del(std::unique_ptr<BinaryNode>& current_mod_node)
		{
			// 操作其父节点删除它自己
			auto parent_node = current_mod_node->GetTheParentUnique();
			if (parent_node->GetLeftChild() == current_mod_node)
			{
				auto nulltmp = std::make_unique<BinaryNode>(nullptr);
				parent_node->SetLeftChild(nulltmp);
			}
			else if (parent_node->GetRightChild() == current_mod_node)
			{
				auto nulltmp = std::make_unique<BinaryNode>(nullptr);
				parent_node->SetRightChild(nulltmp);
			}

			// 删除左子树
			auto nulltmpl = std::make_unique<BinaryNode>(nullptr);
			current_mod_node->SetLeftChild(nulltmpl);

			// 删除右子树
			auto nulltmpr = std::make_unique<BinaryNode>(nullptr);
			current_mod_node->SetRightChild(nulltmpr);
		}

		/// <summary>
		/// 删除树中指定节点
		/// </summary>
		/// <param name="current_mod_node">正在修改的节点</param>
		/// <returns>返回当前建好的树</returns>
		virtual std::vector<std::shared_ptr<BinaryNode> >& del(std::shared_ptr<BinaryNode>& current_mod_node)
		{
			// 操作其父节点删除它自己
			auto parent_node = current_mod_node->GetTheParentUnique();
			if (parent_node->GetLeftChild() == current_mod_node)
			{
				auto nulltmp = std::make_shared<BinaryNode>(nullptr);
				parent_node->SetLeftChild(nulltmp);
			}
			else if (parent_node->GetRightChild() == current_mod_node)
			{
				auto nulltmp = std::make_shared<BinaryNode>(nullptr);
				parent_node->SetRightChild(nulltmp);
			}

			// 删除左子树
			auto nulltmpl = std::make_shared<BinaryNode>(nullptr);
			current_mod_node->SetLeftChild(nulltmpl);

			// 删除右子树
			auto nulltmpr = std::make_shared<BinaryNode>(nullptr);
			current_mod_node->SetRightChild(nulltmpr);
		}

	private:
		bool edge_type;
		std::unique_ptr<BinaryNode> uroot;
		std::shared_ptr<BinaryNode> sroot;
		std::vector<std::unique_ptr<BinaryNode> > uwhole_tree; // 支持树的所有节点可以被线性数据结构存储
		std::vector<std::shared_ptr<BinaryNode> > swhole_tree; // 支持树的所有节点可以被线性数据结构存储
	};
}
#endif // !Binary_Tree_H
