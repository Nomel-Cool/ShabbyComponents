#pragma once
#ifndef COMPRESSING_H
#define COMPRESSING_H
#include <algorithm>
#include <string>
#include <vector>
#include <queue>
#include <algorithm>
#include <unordered_map>

#include "../ShabbyDataStructure/BinaryTree.h"

namespace shabby
{
	class Compressing
	{
	public:
		Compressing() {}
		virtual ~Compressing(){}
	private:
	};

	struct HuffmanTreeNodeForm
	{
		uint32_t freq;
		std::string data;
	};
	/// <summary>
	/// 文本哈夫曼编码
	/// </summary>
	class TextHuffmanCompressing : public Compressing
	{
	public:
		TextHuffmanCompressing()
		{
			ASCII_Frequency.resize(127, 0);
		}
		virtual ~TextHuffmanCompressing() {}
		/// <summary>
		/// 从已有的字符文本建立哈夫曼树
		/// </summary>
		/// <param name="text">字符文本</param>
		/// <returns>哈夫曼二叉树</returns>
		virtual BinaryTree<HuffmanTreeNodeForm> BuildHuffmanTree(std::string text)
		{
			BinaryTree<HuffmanTreeNodeForm> HuffmanTree;
			// 获取文本频率
			CalTextFrequency(text);
			// Huffman树只会取2n-1次节点构造，最后会剩一个
			while (frequency_queue.size() != 1)
			{
				// 设置左节点
				TreeNode<HuffmanTreeNodeForm> left = frequency_queue.top();
				frequency_queue.pop();

				// 设置右节点
				TreeNode<HuffmanTreeNodeForm> right = frequency_queue.top();
				frequency_queue.pop();

				// 设置父节点
				HuffmanTreeNodeForm parent_value;
				parent_value.data = left.GetData().data + right.GetData().data;
				parent_value.freq = left.GetData().freq + right.GetData().freq;
				TreeNode<HuffmanTreeNodeForm> parent(parent_value);
				parent.SetLeftChild(&left);
				parent.SetRightChild(&right);

				// 建树
				HuffmanTree.BuildByAdding(left);
				HuffmanTree.BuildByAdding(right);

				// 父节点重新入队
				frequency_queue.push(parent_value);
			}

			// 显式设置父节点
			HuffmanTree.SetRootNode(frequency_queue.top());

			// 清空缓冲，等待复用（由于priority_queue没有clear函数）
			std::priority_queue<HuffmanTreeNodeForm> empty_pq;
			frequency_queue.swap(empty_pq);
			ASCII_Frequency.clear();

			// 返回建好的哈夫曼树
			return HuffmanTree;
		}
		virtual void interfere_action(TreeNode<HuffmanTreeNodeForm>* last_node, TreeNode<HuffmanTreeNodeForm>* current_node, std::string compressed_string, std::unordered_map<char,std::string>& map)
		{
			if (last_node == nullptr)
				return;
			// 左转为0
			if (last_node->GetLeftChild() == current_node)
				compressed_string += "0";
			// 右转为1
			else if (last_node->GetRightChild() == current_node)
				compressed_string += "1";
			
		}
		/// <summary>
		/// 将已经建好的哈夫曼树转换成字典映射，最后将原文本输出为压缩文本
		/// </summary>
		/// <param name="huffman_tree">已经建立完毕的哈夫曼树</param>
		/// <returns></returns>
		virtual std::string GetHuffmanCode(BinaryTree<HuffmanTreeNodeForm> huffman_tree)
		{
			auto preorder_list = huffman_tree.preorderTraversal(interfere_action);
		}
		/// <summary>
		/// 计算文本频率，并正序排序
		/// </summary>
		/// <param name="text">待统计的文本</param>
		virtual void CalTextFrequency(std::string text)
		{
			// 频率计数
			for (int i = 0; i < text.size(); ++i)
			{
				++ASCII_Frequency[text.at(i)];
			}
			// 最大堆排序
			for (int j = 0; j < sizeof(ASCII_Frequency); j++)
			{
				HuffmanTreeNodeForm node;
				node.freq = ASCII_Frequency[j];
				node.data = (char)j;
				frequency_queue.push(node);
			}
		}
		virtual uint32_t CalWeight() {}
	protected:
		std::vector<uint32_t> ASCII_Frequency; //ASCII码编码
		std::priority_queue<HuffmanTreeNodeForm> frequency_queue;
	private:

	};
}
#endif // !COMPRESSING_H
