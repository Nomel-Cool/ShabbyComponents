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
	/// ���������ڵ㹹�죬�����ַ���ʹ��Ƶ�ʣ��Լ���Ӧ���ı������µ�haffuman����
	/// </summary>
	class HuffmanTreeNodeForm
	{
	public:
		HuffmanTreeNodeForm() : freq(0), origin_char("") {}
		uint32_t freq;
		std::string origin_char;
		std::vector<bool> huffman_code;
		// ���ȶ��ж��ڸ���������Ҫ���رȽϷ���
		bool operator>(const HuffmanTreeNodeForm& other) const {
			return freq > other.freq;
		}
	};

	/// <summary>
	/// �ı����������룬������һ�ö�����
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
		/// �����㷨����
		/// </summary>
		virtual void ResetCache()
		{
			// ��ջ��壬�ȴ����ã�����priority_queueû��clear������
			std::priority_queue<std::shared_ptr<BinaryNode<HuffmanTreeNodeForm> >,
				std::vector<std::shared_ptr<BinaryNode<HuffmanTreeNodeForm> > >,
				std::greater<std::shared_ptr<BinaryNode<HuffmanTreeNodeForm> > > > empty_pq;
			frequency_queue.swap(empty_pq);
			ASCII_Frequency.clear();
		}

		/// <summary>
		/// ��ȡ��Ӧ�Ĺ�����ѹ���ַ���
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
			// ������һ���ַ���boolֵ����8����ʹ��0����䣬��¼���һ���ַ�����Чλ��
			valid_count = bit_count;
			// ������һ���ַ�������Чλ����Ž�ȥ
			if (valid_count > 0)
				compressed_code += temp_char;
			return compressed_code;
		}

		virtual uint32_t CalWeight() { return 0; }
	protected:
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
				std::shared_ptr<BinaryNode<HuffmanTreeNodeForm> > left = std::make_shared<BinaryNode<HuffmanTreeNodeForm> >(left_value);
				frequency_queue.pop();

				// �����ҽڵ�
				auto right_value = frequency_queue.top();
				std::shared_ptr<BinaryNode<HuffmanTreeNodeForm> > right = std::make_shared<BinaryNode<HuffmanTreeNodeForm> >(right_value);
				frequency_queue.pop();

				// ���ø��ڵ�
				HuffmanTreeNodeForm parent_value;
				parent_value.origin_char = left->GetData().origin_char + right->GetData().origin_char;
				parent_value.freq = left->GetData().freq + right->GetData().freq;
				std::shared_ptr<BinaryNode<HuffmanTreeNodeForm> > parent = std::make_shared<BinaryNode<HuffmanTreeNodeForm> >();
				parent->SetData(parent_value);
				parent->SetTheParent(parent);
				parent->SetLeftChild(left);
				parent->SetRightChild(right);

				// ���ø��ӹ�ϵ
				left->SetTheParent(parent);
				right->SetTheParent(parent);

				// ����
				m_huffman_tree.insert(*left);
				m_huffman_tree.insert(*right);

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
		virtual void GetHuffmanMap()
		{
			std::unordered_map<std::string, HuffmanTreeNodeForm> huffman_dictionary;
			auto tree_struct = m_huffman_tree.AccessTreeStruct();
			for (auto& node : tree_struct)
			{
				if (node->GetLeftChild() == nullptr && node->GetRightChild() == nullptr)
				{
					auto r_data = node->GetData(); // ����Ҷ�ӽڵ����ݸ���
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
			// �ͷ����νṹ���ж�ָ̬��
			//m_huffman_tree.FreeTree();
			m_huffman_dictionary = huffman_dictionary;
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
					value.origin_char = std::string(1, (char)j);
					std::shared_ptr<BinaryNode<HuffmanTreeNodeForm> > node = std::make_shared<BinaryNode<HuffmanTreeNodeForm> >();
					node->SetData(value);
					frequency_queue.push(node);
				}
			}
		}

		std::vector<uint32_t> ASCII_Frequency; //ASCII�����
		std::priority_queue<std::shared_ptr<BinaryNode<HuffmanTreeNodeForm> >,
			std::vector<std::shared_ptr<BinaryNode<HuffmanTreeNodeForm> > >,
			std::greater<std::shared_ptr<BinaryNode<HuffmanTreeNodeForm> > > > frequency_queue;
	private:
		BinaryTree<HuffmanTreeNodeForm> m_huffman_tree;
		std::unordered_map<std::string, HuffmanTreeNodeForm> m_huffman_dictionary;
	};
}
#endif // !COMPRESSING_H
