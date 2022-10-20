#include "Utilities.h"


string Utilities::ToUpperCase(const string& inputString) 
{
	string upperCaseCommand = "";

	for (size_t i = 0; i < inputString.size(); i++)
	{
		char currentLetter = inputString[i];
		if (currentLetter > 96 && currentLetter < 123) //lower case ascii
		{
			upperCaseCommand += currentLetter - 32;
		}
		else
		{
			upperCaseCommand += currentLetter;
		}
	}

	return upperCaseCommand;
}

uint16_t Utilities::SwitchEndianness(const uint16_t& inputValue) 
{
	return inputValue << 8 | inputValue >> 8;
}

string Utilities::ConcatenateStrings(vector<string> lineToConcat, char delimitingCharacter)
{
	string result;

	size_t lineToConcatSize = lineToConcat.size();
	for (size_t i = 0; i < lineToConcatSize; ++i)
	{
		if (i != lineToConcatSize - 1) // Its not the last one
		{
			result += (lineToConcat[i] + delimitingCharacter);
		} 
		else 
		{
			result += lineToConcat[i];
		}
	}

	return result;
}

