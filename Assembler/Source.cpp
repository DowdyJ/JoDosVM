
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
#include <exception>

using std::vector;
using std::string;


int main(int argc, char* argv[])
{
	if (argc < 3)
	{
		std::cout << "Usage: "<< argv[0] << " path swap_endianness\n"
			<< "  path:             relative or abolute path to input assembly code using forward slashes.\n" 
			<< "  swap_endianness:  whether to swap byte order during assembly. Acceptable values are TRUE or FALSE. Should typically be TRUE."
			<< std::endl;

		return 1;
	}

	bool switchEndianness;

	if (Utilities::ToUpperCase(argv[2]) == "TRUE") 
		switchEndianness = true;
	else if (Utilities::ToUpperCase(argv[2]) == "FALSE")
		switchEndianness = false;
	else
	{
		std::cout << "Usage: "<< argv[0] << " path swap_endianness\n"
			<< "  path:             relative or abolute path to input assembly code using forward slashes.\n" 
			<< "  swap_endianness:  whether to swap byte order during assembly. Acceptable values are TRUE or FALSE. Should typically be TRUE."
			<< std::endl;

		return 1;
	}

	string inputFilePath = argv[1];

	std::ifstream input(inputFilePath, std::ios::in);
	if (!input.is_open())
	{
		std::cout << "File failed to load at " + inputFilePath + ". Exiting..." << std::endl;
		return -1;
	}

	vector<string> fileAsLines;
	string currentLine;
	std::regex nonBlankLinePattern("(\\w+)", std::regex_constants::ECMAScript);
	while (std::getline(input, currentLine))
	{
		Assembler::RemoveCommentsFromLine(currentLine);

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


	Assembler::HandleENDMacro(fileAsLines);
	Assembler::HandleORIGMacro(fileAsLines);

	vector<vector<string>> tokenizedInput = Assembler::GetTokenizedInputStrings(fileAsLines);
	
	std::cout << "\n------------------------------\nParsed the following tokenized output: \n" << std::endl;

	for (vector<string> const& lineOfTokens : tokenizedInput)
	{
		for (string const& token : lineOfTokens)
			std::cout << token << " ";
		
		std::cout << std::endl;
	}

	Assembler::HandleFILLMacros(tokenizedInput);
	Assembler::HandleTRAPCodeMacroReplacement(tokenizedInput);
	Assembler::HandleSTRINGZMacros(tokenizedInput);

	Assembler::ResolveAndReplaceLabels(tokenizedInput);

	std::cout << "\n------------------------------\nInterpreted the following parsed output: \n" << std::endl;

	
		for (size_t lineIndex = 0; lineIndex < tokenizedInput.size(); ++lineIndex) 
		{
			std::cout << "[" << lineIndex << "]" << Utilities::ConcatenateStrings(tokenizedInput[lineIndex], ' ');
			try {
				if (tokenizedInput[lineIndex][0] == "LIT" && Assembler::IsANumberString(tokenizedInput[lineIndex][1]))
					if (uint16_t argAsInt = Assembler::ConvertStringIfNumber(tokenizedInput[lineIndex][1]); argAsInt < 256)
						std::cout << ' ' << static_cast<char>(argAsInt);
			} 
			catch (const std::invalid_argument& e)
			{
				std::cout << "Exception in line " << lineIndex << ". Tried to convert " << tokenizedInput[lineIndex][1] << " to int." << std::endl;
				std::cout << "Full details: " << e.what() << std::endl;
			}
			
			std::cout << std::endl;
		}
	

	std::cout << "Beginning conversion to binary..." << std::endl;

	vector<uint16_t> outputOfAssembler = Assembler::AssembleIntoBinary(tokenizedInput);
	
	if (Assembler::AreErrors())
	{
		std::cout << "Errors occurred during assembly. Details below:" << std::endl;
		ConsoleLogger logger = ConsoleLogger();

		Assembler::LogErrors(logger);

		std::cout << "\n\nAssembly aborted." << std::endl;
		return 1;
	}
	else 
	{
		std::cout << "No errors were encountered during assembly.";
	}

	if (switchEndianness)
		for (uint16_t& value : outputOfAssembler)
			value = Utilities::SwitchEndianness(value);


	std::ofstream output("ASSEMBLY.obj", std::ios::binary | std::ios::trunc);

	if (output.is_open())
	{
		output.write(reinterpret_cast<char*>(&outputOfAssembler[0]), outputOfAssembler.size() * sizeof(uint16_t));
		
		output.close();

		std::cout << "Assembly complete. Output saved as ASSEMBLY.obj" << std::endl;
	}

	return 0;
}