
#include <stdio.h>
#include <bitset>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <regex>
#include "Assembler.h"
#include "ConsoleLogger.h"
#include "Utilities.h"

using std::vector;
using std::string;


int main(int argc, char* argv[])
{
	bool switchEndianness = false;

	if (argc < 2)
	{
		std::cout << "Need relative path to input file!" << std::endl;
		return 1;
	}
	string inputFilePath = argv[1];

	std::ifstream input(inputFilePath, std::ios::in);
	if (!input.is_open())
	{
		std::cout << "FIle failed to load at " + inputFilePath + ". Exiting..." << std::endl;
		return -1;
	}

	vector<string> fileAsLines;
	string currentLine;
	std::regex nonBlankLinePattern("(\\w+)", std::regex_constants::ECMAScript);
	while (std::getline(input, currentLine))
	{
		std::smatch sm;
		if (std::regex_search(currentLine, sm, nonBlankLinePattern))
		{
			fileAsLines.push_back(currentLine);
		}
	}

	input.close();

	std::cout << "Recieved the following raw input: " << std::endl;

	for (string const& line : fileAsLines)
		std::cout << line << '\n';


	for (string& line : fileAsLines) 
		Assembler::RemoveCommentsFromLine(line);

	Assembler::RemoveAllTextAfterAndIncludingENDMacro(fileAsLines);
	Assembler::HandleORIGMacro(fileAsLines);
	Assembler::HandleFILLMacros(fileAsLines);

	vector<vector<string>> tokenizedInput = Assembler::GetTokenizedInputStrings(fileAsLines);
	Assembler::HandleTRAPCodeMacroReplacement(tokenizedInput);
	Assembler::HandleSTRINGZMacros(tokenizedInput);

	Assembler::ResolveAndReplaceLabels(tokenizedInput);

	std::cout << "\nRecieved the following parsed input: " << std::endl;

	for (size_t lineIndex = 0; lineIndex < tokenizedInput.size(); ++lineIndex) 
	{
		std::cout << Utilities::ConcatenateStrings(tokenizedInput[lineIndex], ' ') << std::endl;
	}



	vector<uint16_t> outputOfAssembler = Assembler::AssembleIntoBinary(tokenizedInput);
	
	if (Assembler::AreErrors())
	{
		ConsoleLogger logger = ConsoleLogger();

		Assembler::LogErrors(logger);
		return 1;
	}
	else 
	{
		std::cout << "No errors were encountered during assembly.";
	}

	if (switchEndianness)
		for (uint16_t& value : outputOfAssembler)
			value = Utilities::SwitchEndianness(value);


	std::ofstream output("ASSEMBLY", std::ios::binary | std::ios::trunc);

	if (output.is_open())
	{
		output.write(reinterpret_cast<char*>(&outputOfAssembler[0]), outputOfAssembler.size() * sizeof(uint16_t));
		
		output.close();

		std::cout << "Assembly complete." << std::endl;
	}

	return 0;
}