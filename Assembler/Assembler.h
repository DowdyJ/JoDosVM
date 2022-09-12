#pragma once
#include <string>
#include <vector>

#define private public


using std::vector;
using std::string;

class Assembler 
{
public:
	static void AssembleIntoBinary(const vector<vector<string>>& inputTokens);

	static vector<vector<string>> GetTokenizedInputStrings(const vector<string>& inputString);

	static void RemoveCommentsFromLine(string& inputLine);

	static void RemoveAllTextAfterAndIncludingENDMacro(vector<string>& linifiedFile);

	static void RemoveExcessWhiteSpaceFromLine(string& inputLine);

	void AddError(const string& errorMessage);

	static void LogErrors(std::vector<std::string>& errorString);

private:
	static vector<string> _errors;

	static bool IsADecimalNumber(const string& token);

	static uint16_t Get5BitImm5(const string& token);

	static int ConvertToDecimal(const string& token, vector<string>& errors);
	static uint16_t ConvertRegisterStringsTo3BitAddress(const string& registerName, vector<string>& errors);

	static uint16_t HandleADDConversion(const vector<string>& instruction);
	static uint16_t HandleANDConversion(const vector<string>& instruction);
	static uint16_t HandleNOTConversion(const vector<string>& instruction);
	static uint16_t HandleBRConversion(const vector<string>& instruction);
	static uint16_t HandleJMPConversion(const vector<string>& instruction);
	static uint16_t HandleJSRConversion(const vector<string>& instruction);
	static uint16_t HandleLDConversion(const vector<string>& instruction);
	static uint16_t HandleLDIConversion(const vector<string>& instruction);
	static uint16_t HandleLDRConversion(const vector<string>& instruction);
	static uint16_t HandleLEAConversion(const vector<string>& instruction);
	static uint16_t HandleSTConversion(const vector<string>& instruction);
	static uint16_t HandleSTIConversion(const vector<string>& instruction);
	static uint16_t HandleRTIConversion(const vector<string>& instruction);
	static uint16_t HandleSTRConversion(const vector<string>& instruction);
	static uint16_t HandleTRAPConversion(const vector<string>& instruction);
};