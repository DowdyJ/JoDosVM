#pragma once
#include <string>
#include <vector>

using std::vector;
using std::string;

class Assembler 
{
public:

	static vector<vector<string>> GetTokenizedInputStrings(const vector<string>& inputString);

	static void RemoveCommentsFromLine(string& inputLine);

	static void RemoveAllTextAfterAndIncludingENDMacro(vector<string>& linifiedFile);

	static void RemoveExcessWhiteSpaceFromLine(string& inputLine);

	void AddError(const string& errorMessage);

	static void LogErrors(std::vector<std::string>& errorString);

private:
	vector<string> _errors;
};