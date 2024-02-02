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
		/// �����е��ַ��ı�������������
		/// </summary>
		/// <param name="text">�ַ��ı�</param>
		/// <returns>������������</returns>
		virtual BinaryTree<HuffmanTreeNodeForm> BuildHuffmanTree(std::string text)
		{
			BinaryTree<HuffmanTreeNodeForm> HuffmanTree;
			// ��ȡ�ı�Ƶ��
			CalTextFrequency(text);
			// Huffman��ֻ��ȡ2n-1�νڵ㹹�죬����ʣһ��
			while (frequency_queue.size() != 1)
			{
				// ������ڵ�
				TreeNode<HuffmanTreeNodeForm> left = frequency_queue.top();
				frequency_queue.pop();

				// �����ҽڵ�
				TreeNode<HuffmanTreeNodeForm> right = frequency_queue.top();
				frequency_queue.pop();

				// ���ø��ڵ�
				HuffmanTreeNodeForm parent_value;
				parent_value.data = left.GetData().data + right.GetData().data;
				parent_value.freq = left.GetData().freq + right.GetData().freq;
				TreeNode<HuffmanTreeNodeForm> parent(parent_value);
				parent.SetLeftChild(&left);
				parent.SetRightChild(&right);

				// ����
				HuffmanTree.BuildByAdding(left);
				HuffmanTree.BuildByAdding(right);

				// ���ڵ��������
				frequency_queue.push(parent_value);
			}

			// ��ʽ���ø��ڵ�
			HuffmanTree.SetRootNode(frequency_queue.top());

			// ��ջ��壬�ȴ����ã�����priority_queueû��clear������
			std::priority_queue<HuffmanTreeNodeForm> empty_pq;
			frequency_queue.swap(empty_pq);
			ASCII_Frequency.clear();

			// ���ؽ��õĹ�������
			return HuffmanTree;
		}
		virtual void interfere_action(TreeNode<HuffmanTreeNodeForm>* last_node, TreeNode<HuffmanTreeNodeForm>* current_node, std::string compressed_string, std::unordered_map<char,std::string>& map)
		{
			if (last_node == nullptr)
				return;
			// ��תΪ0
			if (last_node->GetLeftChild() == current_node)
				compressed_string += "0";
			// ��תΪ1
			else if (last_node->GetRightChild() == current_node)
				compressed_string += "1";
			
		}
		/// <summary>
		/// ���Ѿ����õĹ�������ת�����ֵ�ӳ�䣬���ԭ�ı����Ϊѹ���ı�
		/// </summary>
		/// <param name="huffman_tree">�Ѿ�������ϵĹ�������</param>
		/// <returns></returns>
		virtual std::string GetHuffmanCode(BinaryTree<HuffmanTreeNodeForm> huffman_tree)
		{
			auto preorder_list = huffman_tree.preorderTraversal(interfere_action);
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
		std::vector<uint32_t> ASCII_Frequency; //ASCII�����
		std::priority_queue<HuffmanTreeNodeForm> frequency_queue;
	private:

	};
}
#endif // !COMPRESSING_H
