#include <cstdint>
#include <iostream>
#include <bitset>
#include <fstream>
#include <vector>
#include "Register.h"
#include "ExternalUtilities.h"
#include "Utilities.h"
#include <stdio.h>
#include <stdint.h>


#define PC_START 0x3000


bool ReadLC3ObjFile() 
{
	return true;
}



int main(int argc, char* argv[])
{
	uint16_t dog[200] = {};
	Utilities::LoadFileInto("ASSEMBLY", &dog[0], 200);

	for (size_t i = 0; i < 200; i++)
	{
		std::cout << std::to_string(dog[i]) << std::endl;
	}

	std::cout << "Finished reading file UwU" << std::endl;

	if (argc < 2)
	{
		std::cout << "Usage: LC3 path/to/file.obj" << std::endl;
		return 2;
	}
	else 
	{
		for (size_t i = 1; i < argc; ++i)
		{
			if (!ReadLC3ObjFile())
			{
				std::cout << "Error reading file at: " << argv[i] << std::endl;
			}
		}
	}


	ExternalUtilities EUtils;

	EUtils.Init();

	Register::SetValueInRegister(Register::R_PC, PC_START);

	Register::shouldBeRunning = true;
	while (Register::shouldBeRunning)
	{
	
	
	}






	//Register::SetValueInRegister(Register::R_PC, 0x3000);
	std::ifstream input("TEST", std::ios::binary | std::ios::in);
	
	
	std::vector<uint16_t> buffer;

	input.seekg(0, std::ios::end);
	size_t filesize = input.tellg();
	input.seekg(0, std::ios::beg);

	buffer.resize(filesize / sizeof(uint16_t));

	input.read((char*)buffer.data(), filesize);
	
	for (auto const& ui : buffer)
		std::cout << std::bitset<16>(ui) << ' ';

	input.close();

	std::cout << '\n';


	EUtils.CleanUp();
	

	return 0;
}
