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


