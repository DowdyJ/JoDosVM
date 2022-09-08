#include <cstdint>
#include <iostream>
#include <bitset>
#include <vector>
#include <string>

using std::vector;
using std::string;

vector<vector<string>> GetTokenizedInputStrings(const vector<string>& inputString)
{
	//if has any that constains a period, add error
	//Assume labels and macros have been processed, comments removed

	//   RET R5,    R6 , #LITTERALNUMBER
	vector<vector<string>> tokenizedInput = {};

	for (size_t lineIndex = 0; lineIndex < inputString.size(); lineIndex++)
	{
		string currentLineOfInput = inputString[lineIndex];
		vector<string> tokenizedLine = {};

		for (size_t characterIndex = 0; characterIndex < currentLineOfInput.size(); ++characterIndex)
		{
			if (currentLineOfInput[characterIndex] == ' ' || currentLineOfInput[characterIndex] == ',')
				continue;

			string currentToken = "";
			
			for (;
				characterIndex < currentLineOfInput.size() && currentLineOfInput[characterIndex] != ' ' && currentLineOfInput[characterIndex] != ',';
				++characterIndex)
			{
				currentToken += currentLineOfInput[characterIndex];
			}

			tokenizedLine.push_back(currentToken);
		}

		tokenizedInput.push_back(tokenizedLine);
	}

	return tokenizedInput;
}


uint16_t ExtendSign(uint16_t value, uint16_t bits) 
{
	if ((value >> (bits - 1)) & 1)
	{
		value = ~value;
		value += 1;
		
	}

	std::cout << std::bitset<16>(value) << std::endl;
	return value;
}


int main() 
{
	vector<string> linifiedInput = { "    THIS ,,,,IS AN, EXAMPLE", "S,ET, OF ARGS,", ",THAT M,IGHT MAKE IT THROUGH", "THE   ,  TOKENIZER" };

	auto output = GetTokenizedInputStrings(linifiedInput);

	
	return 0;
}