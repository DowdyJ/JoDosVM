#include "Assembler.h"



vector<vector<string>> Assembler::GetTokenizedInputStrings(const vector<string>& inputString)
{
	//if has any that constains a period, add error
	//Assume labels and macros have been processed, comments removed

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

void Assembler::RemoveCommentsFromLine(string& inputLine)
{
	size_t indexOfSemicolon = inputLine.find(';');
	inputLine = inputLine.substr(0, indexOfSemicolon);
	return;
}

void Assembler::RemoveAllTextAfterAndIncludingENDMacro(vector<string>& linifiedFile)
{
	for (size_t i = 0; i < linifiedFile.size(); ++i)
	{
		string valueAtIndex = linifiedFile[i];

		if (valueAtIndex.find(".END") == 0)
		{
			linifiedFile.erase(linifiedFile.begin() + i, linifiedFile.end());
			break;
		}
	}

	return;
}

void Assembler::RemoveExcessWhiteSpaceFromLine(string& inputLine)
{
}

void Assembler::AddError(const string& errorMessage)
{
	_errors.push_back(errorMessage);
}

void Assembler::LogErrors(std::vector<std::string>& errorString)
{
	
}
