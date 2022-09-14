
#include <stdio.h>
#include <bitset>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include "Assembler.h"
#include "ConsoleLogger.h"
#include "Utilities.h"

using std::vector;
using std::string;


int main(int argc, char* argv[])
{
	vector<string> fileAsLines = { "ADD R5, R1, #3", "RET;Test!", "RET" };

	Assembler::RemoveAllTextAfterAndIncludingENDMacro(fileAsLines);

	for (string& line : fileAsLines) 
		Assembler::RemoveCommentsFromLine(line);

	//for (const auto& var : fileAsLines) std::cout << var << std::endl;

	vector<vector<string>> tokenizedInput = Assembler::GetTokenizedInputStrings(fileAsLines);

	//for (const auto& var : tokenizedInput) 	for (const auto& var1 : var) std::cout << var1 << std::endl;



	
	vector<uint16_t> outputOfAssembler = Assembler::AssembleIntoBinary(tokenizedInput);
	
	if (Assembler::AreErrors())
	{
		ConsoleLogger logger = ConsoleLogger();

		Assembler::LogErrors(logger);
		return 1;
	}

	for (auto const& opcode : outputOfAssembler)
	{
		auto bits = std::bitset<16>(opcode).to_string();
		std::cout << bits << std::endl;
	}


	for (uint16_t& value : outputOfAssembler)
	{
		value = Utilities::SwitchEndianness(value);
	}


	std::ofstream output("ASSEMBLY", std::ios::binary | std::ios::trunc);

	output.write(reinterpret_cast<char*>(&outputOfAssembler[0]), outputOfAssembler.size() * sizeof(uint16_t));

	output.close();

	//Register::SetValueInRegister(Register::R_PC, 0x3000);
	std::ifstream input("ASSEMBLY", std::ios::binary | std::ios::in);


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