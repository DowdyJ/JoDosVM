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


int main(int argc, char* argv[])
{
	if (argc < 2)
	{
		std::cout << "Usage: LC3 path/to/file.obj" << std::endl;
		return 2;
	}

	uint16_t executableOrigin = Utilities::LoadFileInto(argv[1], Register::memory, MEM_MAX, true);

	std::cout << "Finished loading file." << std::endl;


	ExternalUtilities EUtils;

	EUtils.Init();

	Register::SetValueInRegister(Register::R_PC, executableOrigin);

	Register::shouldBeRunning = true;

	Register::ProcessProgram();

/*
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

*/
	EUtils.CleanUp();
	

	return 0;
}
