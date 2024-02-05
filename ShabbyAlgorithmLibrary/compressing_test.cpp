#include <iostream>
#include "../ShabbyAlgorithmLibrary/Compressing.h"
using namespace shabby;

int main()
{
	std::string msg = "Hello World";
	TextHuffmanCompressing huffman_compressing;
	int padding = 0;
	std::string compressed_code = huffman_compressing.GetHuffmanCompressedText(msg, padding);

    std::cout << compressed_code << " ------ " << compressed_code.size() * 8 - padding << " ------ " << padding << std::endl;
	return 0;
}