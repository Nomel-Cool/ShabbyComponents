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
		// ���ȶ��ж��ڸ���������Ҫ���رȽϷ���
		bool operator>(const HuffmanTreeNodeForm& other) const {
			return freq > other.freq;
		}
	};
	/// <summary>
	/// �ı�����������
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
		/// �����㷨����
		/// </summary>
		virtual void ResetCache()
		{
			// ��ջ��壬�ȴ����ã�����priority_queueû��clear������
			std::priority_queue<TreeNode<HuffmanTreeNodeForm>*,
				std::vector<TreeNode<HuffmanTreeNodeForm>*>,
				std::greater<TreeNode<HuffmanTreeNodeForm>*>> empty_pq;
			frequency_queue.swap(empty_pq);
			ASCII_Frequency.clear();
		}
		/// <summary>
		/// �����е��ַ��ı�������������
		/// </summary>
		/// <param name="text">�ַ��ı�</param>
		/// <returns>������������</returns>
		virtual void BuildHuffmanTree(std::string text)
		{
			// ��ȡ�ı�Ƶ��
			CalTextFrequency(text);

			// Huffman��ֻ��ȡ2n-1�νڵ㹹�죬����ʣһ��
			while (frequency_queue.size() != 1)
			{
				// ������ڵ�
				auto left_value = frequency_queue.top();
				TreeNode<HuffmanTreeNodeForm>* left = left_value;
				frequency_queue.pop();

				// �����ҽڵ�
				auto right_value = frequency_queue.top();
				TreeNode<HuffmanTreeNodeForm> *right = right_value;
				frequency_queue.pop();

				// ���ø��ڵ�
				HuffmanTreeNodeForm parent_value;
				parent_value.origin_char = left->GetData().origin_char + right->GetData().origin_char;
				parent_value.freq = left->GetData().freq + right->GetData().freq;
				TreeNode<HuffmanTreeNodeForm> *parent = new TreeNode<HuffmanTreeNodeForm>();
				parent->SetData(parent_value);
				parent->SetParentNode(parent);
				parent->SetLeftChild(left);
				parent->SetRightChild(right);

				// ���ø��ӹ�ϵ
				left->SetParentNode(parent);
				right->SetParentNode(parent);

				// ����
				m_huffman_tree.BuildByAdding(left);
				m_huffman_tree.BuildByAdding(right);

				// ���ڵ��������
				frequency_queue.push(parent);
			}

			// ��ʽ���ø��ڵ�
			m_huffman_tree.SetRootNode(frequency_queue.top());
		}
		/// <summary>
		/// ���Ѿ����õĹ�������ת�����ֵ�ӳ�䣬���ԭ�ı����Ϊѹ���ı�
		/// </summary>
		/// <param name="huffman_tree">�Ѿ�������ϵĹ�������</param>
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
		/// �����ı�Ƶ�ʣ�����������
		/// </summary>
		/// <param name="text">��ͳ�Ƶ��ı�</param>
		virtual void CalTextFrequency(std::string text)
		{
			// Ƶ�ʼ���
			for (int i = 0; i < text.size(); ++i)
			{
				++ASCII_Frequency[text.at(i)];
			}
			// ��������
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
		std::vector<uint32_t> ASCII_Frequency; //ASCII�����
		std::priority_queue<TreeNode<HuffmanTreeNodeForm>*,
			std::vector<TreeNode<HuffmanTreeNodeForm>*>,
			std::greater<TreeNode<HuffmanTreeNodeForm>*>> frequency_queue;
	private:
		BinaryTree<HuffmanTreeNodeForm> m_huffman_tree;
	};
}
#endif // !COMPRESSING_H
