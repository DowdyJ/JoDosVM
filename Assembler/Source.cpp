
#include <stdio.h>
#include <bitset>
#include <vector>
#include <string>
#include "Assembler.h"
#include "ConsoleLogger.h"

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


	if (Assembler::AreErrors())
	{
		ConsoleLogger logger = ConsoleLogger();

		Assembler::LogErrors(logger);
		return 1;
	}
	
	vector<uint16_t> output = Assembler::AssembleIntoBinary(tokenizedInput);
	
	for (auto const& opcode : output) 
	{
		std::cout << opcode << std::endl;
	}
	

	return 0;
}