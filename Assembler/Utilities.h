#pragma once
#include <string>
#include <vector>

using std::vector;
using std::string;

class Utilities 
{
public:
	static string ToUpperCase(const string& inputString);

	static uint16_t SwitchEndianness(const uint16_t& inputValue);

	static string ConcatenateStrings(vector<string> lineToConcat, char delimitingCharacter = ' ');
};