#include <iostream>
#include "../ShabbyAlgorithmLibrary/Compressing.h"
using namespace shabby;

int main()
{
	std::string msg = "Hello World";
	TextHuffmanCompressing huffman_compressing;
	huffman_compressing.BuildHuffmanTree(msg);
	auto huffman_dictionary = huffman_compressing.GetHuffmanCode();
	std::string compressed_str = "";
	for (const char& c : msg)
	{
		std::string t(1, c);
		compressed_str += huffman_dictionary[t];
	}
	std::cout << std::endl;
	for (const auto& pair : huffman_dictionary) {
		std::cout << "Key: " << pair.first << ", Value: " << pair.second << std::endl;
	}
	std::cout << compressed_str << std::endl;
	return 0;
}