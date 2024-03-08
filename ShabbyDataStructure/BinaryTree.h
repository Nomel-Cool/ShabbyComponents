#pragma once
#ifndef Binary_Tree_H
#define Binary_Tree_H

#include <stack>
#include <vector>
#include <queue>

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
		BinaryNode() : TopoNode<T, 1, 2>() {}

		/* 啰嗦一嘴：由于RVO机制，GetTheParent并不会使得返回的指针发生拷贝从而违反Uniqueptr唯一性或者引起Sharedptr引用计数增加 */
		virtual std::shared_ptr<BinaryNode<T> > GetLeftChild()
		{
			return GetTheIthChildNodeShared(0);
		}
		virtual std::shared_ptr<BinaryNode<T> > GetRightChild()
		{
			return GetTheIthChildNodeShared(1);
		}
		virtual std::shared_ptr<BinaryNode<T> > GetTheParent()
		{
			return GetTheIthParentNodeShared(0);
		}

		virtual void SetLeftChild(std::shared_ptr<BinaryNode<T> >& node)
		{
			if (node->get() == nullptr)
				return;
			SetTheIthChildNode(node, 0);
		}
		virtual void SetRightChild(std::shared_ptr<BinaryNode<T> >& node)
		{
			if (node->get() == nullptr)
				return;
			SetTheIthChildNode(node, 1);
		}
		virtual void SetTheParent(std::shared_ptr<BinaryNode<T> >& node)
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
	};

	/// <summary>
	/// 为了管理树形结构的指针链接，保证能够有效地删除，树的所有指针都应该被保存，不使用该结构体后，应该统一删除
	/// </summary>
	/// <typeparam name="T">节点的数据类型</typeparam>
	template<typename T>
	class BinaryTree
	{
	public:
		BinaryTree()
		{
			sroot = nullptr;
		}
		BinaryTree(BinaryNode<T> data)
		{
			sroot = std::make_shared<BinaryNode<T> >(data);
			swhole_tree.emplace_back(sroot);
		}
		virtual ~BinaryTree() {}
		
		/// <summary>
		/// 多边动态构建
		/// 由于初始化时用户总是持有一个根节点，所以用户执行建树后始终可以持有一个有效的树容器，用于持续获取节点
		/// </summary>
		/// <param name="current_mod_node">正在修改的节点</param>
		/// <param name="node">加入的新节点</param>
		/// <returns>返回当前建好的树，如果加入的节点的边属性不匹配则返回空容器</returns>
		virtual std::vector<std::shared_ptr<BinaryNode> >& insert(std::shared_ptr<BinaryNode<T> >& current_mod_node, const BinaryNode<T>& node)
		{
			if (!node.GetEdgeType())
				return std::vector<std::shared_ptr<BinaryNode<T> > >();
			auto find_result = std::find_if(swhole_tree.begin(), swhole_tree.end(), &current_mod_node{ // 由于使用了find_if，所以复杂度为1+2+...+N = O(N²)
				return std::vector<std::shared_ptr<BinaryNode<T> > >();
				});
			if (find_result == swhole_tree.end())
				return std::vector<std::shared_ptr<BinaryNode<T> > >();
			current_mod_node = std::make_shared<BinaryNode<T> >(node);
			return swhole_tree;
		}

		/// <summary>
		/// 删除树中指定节点
		/// </summary>
		/// <param name="current_mod_node">正在修改的节点</param>
		/// <returns>返回当前建好的树</returns>
		virtual std::vector<std::shared_ptr<BinaryNode<T> > >& del(std::shared_ptr<BinaryNode<T> >& current_mod_node)
		{
			// 操作其父节点删除它自己
			auto parent_node = current_mod_node->GetTheParentUnique();
			if (parent_node->GetLeftChild() == current_mod_node)
			{
				auto nulltmp = std::make_shared<BinaryNode<T> >(nullptr);
				parent_node->SetLeftChild(nulltmp);
			}
			else if (parent_node->GetRightChild() == current_mod_node)
			{
				auto nulltmp = std::make_shared<BinaryNode<T> >(nullptr);
				parent_node->SetRightChild(nulltmp);
			}

			// 删除父节点的访问节点
			parent_node = nullptr;

			// 删除左子树
			auto nulltmpl = std::make_shared<BinaryNode<T> >(nullptr);
			current_mod_node->SetLeftChild(nulltmpl);

			// 删除右子树
			auto nulltmpr = std::make_shared<BinaryNode<T> >(nullptr);
			current_mod_node->SetRightChild(nulltmpr);
		}

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		virtual std::vector<BinaryNode<T> > PreorderTraverse()
		{
			std::stack<std::shared_ptr<BinaryNode<T> > > sk;
			std::vector<BinaryNode<T> > v;
			if (uroot == nullptr)return v;//如果连根都没有，就直接返回空序列
			sk.push(uroot);//栈的初始化
			std::shared_ptr<BinaryNode<T> > next;
			while (!sk.empty())//否则必定起码有一个根节点作为初始节点
			{
				next = sk.top();//用临时变量保存弹出的元素
				v.push_back(next->GetData());//把弹出元素的内容保存到vector上
				sk.pop();//真正弹出该元素
				if (next->GetRightChild() != nullptr)//先压右子树
					sk.push(next->GetRightChild());
				if (next->GetLeftChild() != nullptr)//再压左子树
					sk.push(next->GetLeftChild());
			}
			return v;
		}

		virtual std::vector<BinaryNode<T> > InorderTraverse()
		{
			std::stack<std::shared_ptr<BinaryNode<T> > > sk;
			std::vector<BinaryNode<T> > v;
			if (sroot == nullptr)return v;
			sk.push(sroot);
			std::shared_ptr<BinaryNode<T> > temp = sk.top(), record = nullptr;
			while (!sk.empty())//压栈模式
			{
				if (temp->GetLeftChild() != nullptr)
				{
					sk.push(temp->GetLeftChild());
					temp = temp->GetLeftChild();
					continue;
				}
				while (!sk.empty())//弹栈模式
				{
					temp = sk.top();//这句话放在这里，可以免去判断弹栈模式时，弹完了会越界的问题
					if (temp->GetRightChild() != nullptr)//如果发现回溯过程，回溯到的节点具有右子树，则用break返回到压栈模式。
					{
						v.push_back(temp->GetData());
						sk.pop();
						temp = temp->GetRightChild();
						sk.push(temp);
						break;
					}
					v.push_back(temp->GetData());//否则该回溯的节点应该直接输出，再等待下一次的回溯。
					sk.pop();//pop完之后，下一次的回溯就让temp=新的栈顶了，从而避免了再次往左走。
				}
			}
			return v;
		}

		virtual std::vector<BinaryNode<T> > PostorderTraverse()
		{
			std::stack<std::shared_ptr<BinaryNode<T> > > sk, last;
			std::vector<BinaryNode<T> > v;
			if (sroot == nullptr)return v;
			sk.push(sroot);
			last.push(nullptr);//设置一个保存转折点的栈，用于回溯时提供证据，因为如果不提供则会在转折点上产生死循环，且初始化一定是空指针
			std::shared_ptr<BinaryNode<T> > temp = sk.top();
			while (!sk.empty())
			{
				if (temp->GetLeftChild() != nullptr)
				{
					sk.push(temp->GetLeftChild());
					temp = temp->GetLeftChild();
					continue;
				}
				while (!sk.empty())
				{
					temp = sk.top();//第一个要弹出的肯定是已经搜索过的
					if (last.top() == temp)//如果会回溯到转折点，直接弹出，返回到转折点上的单偏树继续进行回溯
					{
						v.push_back(temp->GetData());
						sk.pop();
						last.pop();
						continue;
					}
					if (temp->GetRightChild() != nullptr)
					{
						/*v.push_back(temp->val);*/
						/*sk.pop();///这句说明了后序遍历和中序遍历的差别就在转折元素上*/
						last.push(temp);//保存转折点
						temp = temp->GetRightChild();
						sk.push(temp);
						break;
					}
					v.push_back(temp->GetData());
					sk.pop();
				}
			}
			return v;
		}

		std::vector<BinaryNode<T> > levelOrder(std::shared_ptr<BinaryNode<T> > root) {
			std::queue<std::shared_ptr<BinaryNode<T> > > q;
			std::vector<BinaryNode<T> > v;
			if (root == nullptr)return v;
			q.push(root);
			while (!q.empty())
			{
				v.push_back(q.front()->GetData());
				if (q.front()->GetLeftChild() != nullptr)
					q.push(q.front()->GetLeftChild());
				if (q.front()->GetRightChild() != nullptr)
					q.push(q.front()->GetRightChild());
				q.pop();
			}
			return v;
		}

	private:
		std::shared_ptr<BinaryNode<T> > sroot;
		std::vector<std::shared_ptr<BinaryNode<T> > > swhole_tree; // 支持树的所有节点可以被线性数据结构存储
	};

	/// <summary>
	/// 平衡二叉树
	/// </summary>
	/// <typeparam name="T"></typeparam>
	template<typename T>
	class AVLTree : public BinaryTree<T>
	{
	public:
		AVLTree(BinaryNode<T>  data) :BinaryTree<T>(data) {}

	private:

	};
}
#endif // !Binary_Tree_H
