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


#include <iostream>
namespace shabby
{
	class Compressing
	{
	public:
		Compressing() {}
		virtual ~Compressing(){}
	private:
	};

	class HuffmanTreeNodeForm
	{
	public:
		HuffmanTreeNodeForm() : freq(0), origin_char(""), huffman_code("") {}
		uint32_t freq;
		std::string origin_char;
		std::string huffman_code;
		// 优先队列对于复杂类型需要重载比较符号
		bool operator>(const HuffmanTreeNodeForm& other) const {
			return freq > other.freq;
		}
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
		/// 重置算法缓存
		/// </summary>
		virtual void ResetCache()
		{
			// 清空缓冲，等待复用（由于priority_queue没有clear函数）
			std::priority_queue<TreeNode<HuffmanTreeNodeForm>*,
				std::vector<TreeNode<HuffmanTreeNodeForm>*>,
				std::greater<TreeNode<HuffmanTreeNodeForm>*>> empty_pq;
			frequency_queue.swap(empty_pq);
			ASCII_Frequency.clear();
		}
		/// <summary>
		/// 从已有的字符文本建立哈夫曼树
		/// </summary>
		/// <param name="text">字符文本</param>
		/// <returns>哈夫曼二叉树</returns>
		virtual void BuildHuffmanTree(std::string text)
		{
			// 获取文本频率
			CalTextFrequency(text);

			// Huffman树只会取2n-1次节点构造，最后会剩一个
			while (frequency_queue.size() != 1)
			{
				// 设置左节点
				auto left_value = frequency_queue.top();
				TreeNode<HuffmanTreeNodeForm>* left = left_value;
				frequency_queue.pop();

				// 设置右节点
				auto right_value = frequency_queue.top();
				TreeNode<HuffmanTreeNodeForm> *right = right_value;
				frequency_queue.pop();

				// 设置父节点
				HuffmanTreeNodeForm parent_value;
				parent_value.origin_char = left->GetData().origin_char + right->GetData().origin_char;
				parent_value.freq = left->GetData().freq + right->GetData().freq;
				TreeNode<HuffmanTreeNodeForm> *parent = new TreeNode<HuffmanTreeNodeForm>();
				parent->SetData(parent_value);
				parent->SetParentNode(parent);
				parent->SetLeftChild(left);
				parent->SetRightChild(right);

				// 设置父子关系
				left->SetParentNode(parent);
				right->SetParentNode(parent);

				// 建树
				m_huffman_tree.BuildByAdding(left);
				m_huffman_tree.BuildByAdding(right);

				// 父节点重新入队
				frequency_queue.push(parent);
			}

			// 显式设置父节点
			m_huffman_tree.SetRootNode(frequency_queue.top());
		}
		/// <summary>
		/// 将已经建好的哈夫曼树转换成字典映射，最后将原文本输出为压缩文本
		/// </summary>
		/// <param name="huffman_tree">已经建立完毕的哈夫曼树</param>
		/// <returns></returns>
		virtual std::unordered_map<std::string,std::string> GetHuffmanCode()
		{
			std::unordered_map<std::string, std::string> huffman_dictionary;
			for(auto& node : m_huffman_tree.m_tree)
			{
				if (node->GetLeftChild() == nullptr && node->GetRightChild() == nullptr)
				{
					auto data = node->GetData();
					while (node != node->GetParentNode())
					{
						auto _left = node->GetParentNode()->GetLeftChild();
						auto _right = node->GetParentNode()->GetRightChild();
						if (_left == node)
						{
							data.huffman_code.insert(data.huffman_code.begin(), '0');
						}
						else if (_right == node)
						{
							data.huffman_code.insert(data.huffman_code.begin(), '1');
						}
						node = node->GetParentNode();
					}
					huffman_dictionary.insert(std::pair<std::string,std::string>(data.origin_char, data.huffman_code));
				}
			}
			return huffman_dictionary;
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
			for (int j = 0; j < ASCII_Frequency.size(); j++)
			{
				if (ASCII_Frequency[j] > 0)
				{
					HuffmanTreeNodeForm value;
					value.freq = ASCII_Frequency[j];
					value.origin_char = std::string(1,(char)j);
					TreeNode<HuffmanTreeNodeForm> *node = new TreeNode<HuffmanTreeNodeForm>();
					node->SetData(value);
					frequency_queue.push(node);
				}
			}
		}
		virtual uint32_t CalWeight() { return 0; }
	protected:
		std::vector<uint32_t> ASCII_Frequency; //ASCII码编码
		std::priority_queue<TreeNode<HuffmanTreeNodeForm>*,
			std::vector<TreeNode<HuffmanTreeNodeForm>*>,
			std::greater<TreeNode<HuffmanTreeNodeForm>*>> frequency_queue;
	private:
		BinaryTree<HuffmanTreeNodeForm> m_huffman_tree;
	};
}
#endif // !COMPRESSING_H
