
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
	std::ifstream input("INPUT", std::ios::in);

	vector<string> fileAsLines;
	string currentLine;
	while (std::getline(input, currentLine))
		fileAsLines.push_back(currentLine);

	input.close();

	for (string const& line : fileAsLines)
		std::cout << line << '\n';

	

	Assembler::RemoveAllTextAfterAndIncludingENDMacro(fileAsLines);

	for (string& line : fileAsLines) 
		Assembler::RemoveCommentsFromLine(line);

	vector<vector<string>> tokenizedInput = Assembler::GetTokenizedInputStrings(fileAsLines);
	
	vector<uint16_t> outputOfAssembler = Assembler::AssembleIntoBinary(tokenizedInput);
	
	if (Assembler::AreErrors())
	{
		ConsoleLogger logger = ConsoleLogger();

		Assembler::LogErrors(logger);
		return 1;
	}

	for (uint16_t& value : outputOfAssembler)
	{
		value = Utilities::SwitchEndianness(value);
	}


	std::ofstream output("ASSEMBLY", std::ios::binary | std::ios::trunc);

	if (output.is_open())
	{
		output.write(reinterpret_cast<char*>(&outputOfAssembler[0]), outputOfAssembler.size() * sizeof(uint16_t));
		output.close();
	}

	if (Assembler::labelIndexPairs.size() > 0)
	{
		for (auto itr = Assembler::labelIndexPairs.begin(); itr != Assembler::labelIndexPairs.end(); ++itr)
		{
			std::cout << itr->first << " : " << itr->second << std::endl;
		}
	}



	return 0;
}