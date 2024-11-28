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
	/// <summary>
	/// 哈夫曼树节点构造，保存字符、使用频率，以及对应的文本材料下的haffuman编码
	/// </summary>
	class HuffmanTreeNodeForm
	{
	public:
		HuffmanTreeNodeForm() : freq(0), origin_char("") {}
		uint32_t freq;
		std::string origin_char;
		std::vector<bool> huffman_code;
		// 优先队列对于复杂类型需要重载比较符号
		bool operator>(const HuffmanTreeNodeForm& other) const {
			return freq > other.freq;
		}
	};

	/// <summary>
	/// 文本哈夫曼编码，本质是一棵二叉树
	/// </summary>
	class TextHuffmanCompressing : public BinaryTree<BinaryNode<HuffmanTreeNodeForm> >
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
			std::priority_queue<std::shared_ptr<BinaryNode<HuffmanTreeNodeForm> >,
				std::vector<std::shared_ptr<BinaryNode<HuffmanTreeNodeForm> > >,
				std::greater<std::shared_ptr<BinaryNode<HuffmanTreeNodeForm> > > > empty_pq;
			frequency_queue.swap(empty_pq);
			ASCII_Frequency.clear();
		}

		/// <summary>
		/// 求取对应的哈夫曼压缩字符串
		/// </summary>
		/// <param name="valid_count"></param>
		virtual std::string GetHuffmanCompressedText(std::string msg, int& valid_count)
		{
			BuildHuffmanTree(msg);
			GetHuffmanMap();
			std::string compressed_code = "";
			int bit_count = 0;
			char temp_char = 0;
			for (auto& c : msg)
			{
				auto key = m_huffman_dictionary[std::string(1, c)];
				auto code = key.huffman_code;

				for (int i = 0; i < code.size(); ++i)
				{
					temp_char <<= 1;
					if (code[i])
						temp_char |= 1;
					bit_count++;

					if (bit_count == 8)
					{
						compressed_code += temp_char;
						temp_char = 0;
						bit_count = 0;
					}
				}
			}
			// 如果最后一个字符的bool值不足8个，使用0来填充，记录最后一个字符的有效位数
			valid_count = bit_count;
			// 如果最后一个字符还有有效位，则放进去
			if (valid_count > 0)
				compressed_code += temp_char;
			return compressed_code;
		}

		virtual uint32_t CalWeight() { return 0; }
	protected:
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
				std::shared_ptr<BinaryNode<HuffmanTreeNodeForm> > left = std::make_shared<BinaryNode<HuffmanTreeNodeForm> >(left_value);
				frequency_queue.pop();

				// 设置右节点
				auto right_value = frequency_queue.top();
				std::shared_ptr<BinaryNode<HuffmanTreeNodeForm> > right = std::make_shared<BinaryNode<HuffmanTreeNodeForm> >(right_value);
				frequency_queue.pop();

				// 设置父节点
				HuffmanTreeNodeForm parent_value;
				parent_value.origin_char = left->GetData().origin_char + right->GetData().origin_char;
				parent_value.freq = left->GetData().freq + right->GetData().freq;
				std::shared_ptr<BinaryNode<HuffmanTreeNodeForm> > parent = std::make_shared<BinaryNode<HuffmanTreeNodeForm> >();
				parent->SetData(parent_value);
				parent->SetTheParent(parent);
				parent->SetLeftChild(left);
				parent->SetRightChild(right);

				// 设置父子关系
				left->SetTheParent(parent);
				right->SetTheParent(parent);

				// 建树
				m_huffman_tree.insert(*left);
				m_huffman_tree.insert(*right);

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
		virtual void GetHuffmanMap()
		{
			std::unordered_map<std::string, HuffmanTreeNodeForm> huffman_dictionary;
			auto tree_struct = m_huffman_tree.AccessTreeStruct();
			for (auto& node : tree_struct)
			{
				if (node->GetLeftChild() == nullptr && node->GetRightChild() == nullptr)
				{
					auto r_data = node->GetData(); // 保存叶子节点数据副本
					while (node != node->GetTheParent())
					{
						auto _left = node->GetTheParent()->GetLeftChild();
						auto _right = node->GetTheParent()->GetRightChild();
						if (_left == node)
						{
							r_data.huffman_code.push_back(false);
						}
						else if (_right == node)
						{
							r_data.huffman_code.push_back(true);
						}
						node = node->GetTheParent();
					}
					huffman_dictionary.insert(std::pair<std::string, HuffmanTreeNodeForm>(r_data.origin_char, r_data));
				}
			}
			// 释放树形结构所有动态指针
			//m_huffman_tree.FreeTree();
			m_huffman_dictionary = huffman_dictionary;
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
					value.origin_char = std::string(1, (char)j);
					std::shared_ptr<BinaryNode<HuffmanTreeNodeForm> > node = std::make_shared<BinaryNode<HuffmanTreeNodeForm> >();
					node->SetData(value);
					frequency_queue.push(node);
				}
			}
		}

		std::vector<uint32_t> ASCII_Frequency; //ASCII码编码
		std::priority_queue<std::shared_ptr<BinaryNode<HuffmanTreeNodeForm> >,
			std::vector<std::shared_ptr<BinaryNode<HuffmanTreeNodeForm> > >,
			std::greater<std::shared_ptr<BinaryNode<HuffmanTreeNodeForm> > > > frequency_queue;
	private:
		BinaryTree<HuffmanTreeNodeForm> m_huffman_tree;
		std::unordered_map<std::string, HuffmanTreeNodeForm> m_huffman_dictionary;
	};
}
#endif // !COMPRESSING_H
