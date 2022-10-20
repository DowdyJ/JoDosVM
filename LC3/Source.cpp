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
	
	EUtils.CleanUp();
	

	return 0;
}
