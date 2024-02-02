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
	/// 文本哈夫曼编码
	/// </summary>
	class TextHuffmanCompressing : public Compressing
	{
	public:
		TextHuffmanCompressing() {}
		TextHuffmanCompressing() {}
		virtual BinaryTree BuildHuffmanTree(std::string text)
		{
			// 获取文本频率
			CalTextFrequency(text);

		}
		virtual std::string GetHuffmanCode(BinaryTree huffman_tree) {}
		/// <summary>
		/// 计算文本频率，并正序排序
		/// </summary>
		/// <param name="text">待统计的文本</param>
		virtual void CalTextFrequency(std::string text)
		{
			for (int i = 0; i < text.size(); ++i)
			{
				++ASCII_Frequency[text.at(i)];
			}
		}
		virtual uint32_t CalWeight() {}
	protected:
		uint32_t ASCII_Frequency[127] = {0}; //ASCII码编码
	private:

	};
}
#endif // !COMPRESSING_H
