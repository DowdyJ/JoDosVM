#pragma once
#include <string>
using std::string;

class Utilities 
{
public:
	static string ToUpperCase(const string& inputString);

	static uint16_t SwitchEndianness(const uint16_t& inputValue);
};