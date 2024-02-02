#pragma once
#ifndef COMPRESSING_H
#define COMPRESSING_H
#include <algorithm>
#include <string>
#include <vector>

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

	/// <summary>
	/// �ı�����������
	/// </summary>
	class TextHuffmanCompressing : public Compressing
	{
	public:
		TextHuffmanCompressing() {}
		TextHuffmanCompressing() {}
		virtual BinaryTree BuildHuffmanTree(std::string text)
		{
			// ��ȡ�ı�Ƶ��
			CalTextFrequency(text);

		}
		virtual std::string GetHuffmanCode(BinaryTree huffman_tree) {}
		/// <summary>
		/// �����ı�Ƶ�ʣ�����������
		/// </summary>
		/// <param name="text">��ͳ�Ƶ��ı�</param>
		virtual void CalTextFrequency(std::string text)
		{
			for (int i = 0; i < text.size(); ++i)
			{
				++ASCII_Frequency[text.at(i)];
			}
		}
		virtual uint32_t CalWeight() {}
	protected:
		uint32_t ASCII_Frequency[127] = {0}; //ASCII�����
	private:

	};
}
#endif // !COMPRESSING_H
