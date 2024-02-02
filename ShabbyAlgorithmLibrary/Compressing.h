#pragma once
#ifndef COMPRESSING_H
#define COMPRESSING_H
#include <algorithm>
#include <string>
#include <vector>
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
	/// ÎÄ±¾¹þ·òÂü±àÂë
	/// </summary>
	class TextHuffmanCompressing : public Compressing
	{
	public:
		TextHuffmanCompressing() {}
		TextHuffmanCompressing() {}
		virtual BinaryTree BuildHuffmanTree(std::string text) {}
		virtual std::string GetHuffmanCode(BinaryTree huffman_tree) {}
		virtual uint32_t CalWeight() {}
	protected:

	private:

	};
}
#endif // !COMPRESSING_H
