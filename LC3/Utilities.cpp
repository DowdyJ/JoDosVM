#include <fstream>
#include <vector>
#include <iostream>
#include "Utilities.h"

using std::vector;

void Utilities::LoadFileInto(string filename, uint16_t* test, uint16_t numberToRead)
{
	std::ifstream input(filename, std::ios::in | std::ios::binary);

	input.seekg(0, input.end);
	uint16_t lengthOfFile = input.tellg();
	input.seekg(0, input.beg);
	
	std::cout << "Length of file read as: " << std::to_string(lengthOfFile) << std::endl;

	if (numberToRead > lengthOfFile) 
	{
		std::cout << "File shorter than available space. Reading " << std::to_string(lengthOfFile) << " units instead." << std::endl;
		numberToRead = lengthOfFile;
	}


	for (size_t i = 0; i < numberToRead; ++i)
	{
		input.read(reinterpret_cast<char*>(test), sizeof(uint16_t));
	}

	input.close();

	if (!input.good())
	{
		std::cout << "Oh long johnson" << std::endl;
		return;
	}

	std::cout << "File done being read!" << std::endl;


	return;
}
