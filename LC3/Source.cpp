#include <cstdint>
#include <iostream>
#include <bitset>
#include <fstream>
#include <vector>
#include "Register.h"

int main(int argc, char* argv[])
{
	uint16_t valuesToWrite[] = { 0, 1, 3 };
	
	std::ofstream output("TEST", std::ios::binary | std::ios::trunc);

	output.write(reinterpret_cast<char*>(&valuesToWrite), sizeof(valuesToWrite));

	output.close();

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
	return 0;
}
