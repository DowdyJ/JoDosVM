#include <fstream>
#include <vector>
#include <iostream>
#include "Utilities.h"

using std::vector;

uint16_t Utilities::LoadFileInto(string filename, uint16_t* memory, uint16_t memorySize, bool swapEndianness)
{
	std::ifstream input(filename, std::ios::in | std::ios::binary);

	uint16_t startAddress;
	input.read(reinterpret_cast<char*>(&startAddress), 2);

	if (swapEndianness)
		startAddress = startAddress << 8 | startAddress >> 8;

	std::cout << "Start address read as " << startAddress << std::endl;
	
	input.seekg(0, input.end);
	uint16_t lengthOfFile = input.tellg() / 2;
	input.seekg(0, input.beg);
	
	std::cout << "Length of file read as: " << std::to_string(lengthOfFile) << " words" << std::endl;

	if (memorySize - startAddress > lengthOfFile) 
	{
		std::cout << "File shorter than available space. Reading " << std::to_string(lengthOfFile) << " units instead." << std::endl;
	} else 
	{
		std::cout << "File larger than available space. Aborting..." << std::endl;
		input.close();
		return 0;
	}


	for (size_t i = 0; i < lengthOfFile; ++i)
	{
		input.read(reinterpret_cast<char*>(memory + startAddress), sizeof(uint16_t));
	}

	if (swapEndianness)
	{
		for (size_t i = 0; i < lengthOfFile; ++i)
		{
			(memory)[startAddress + i] = ((memory)[startAddress + i] << 8) | ((memory)[startAddress + i] >> 8);
		}
	}

	input.close();

	if (!input.good())
	{
		std::cout << "File read with errors!" << std::endl;
	}

	std::cout << "File done being read." << std::endl;


	return startAddress;
}
