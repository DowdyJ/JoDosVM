#pragma once
#include <map>
#include <algorithm>
#include <string>
#include <vector>
#include <sstream>
#include <iostream>

#include "Logger.h"

using std::map;
using std::vector;
using std::string;

class Assembler 
{
public:
	static vector<uint16_t> AssembleIntoBinary(const vector<vector<string>>& inputTokens);

	static void ResolveAndReplaceLabels(vector<vector<string>>& inputTokens);

	static vector<vector<string>> GetTokenizedInputStrings(const vector<string>& inputString);

	static void RemoveCommentsFromLine(string& inputLine);

	static void HandleENDMacro(vector<string>& linifiedFile);
	static void HandleORIGMacro(vector<string>& linifiedFile);

	static void HandleFILLMacros(vector<vector<string>>& tokeninzedInput);
	static void HandleSTRINGZMacros(vector<vector<string>>& tokeninzedInput);
	static void HandleTRAPCodeMacroReplacement(vector<vector<string>>& tokeninzedInput);

	static void LogErrors(Logger& logger);
	static bool AreErrors();

private:
	static vector<string> _errors;

	static map<string, uint16_t> BuildLabelAddressMap(vector<vector<string>>& inputTokens, vector<string>& errors);
	static void ReplaceLabelsWithOffsets(vector<vector<string>>& inputTokens, const vector<string>& opCodesToCheck, const map<string, uint16_t>& labelIndexPairs, vector<string>& errors);

	static bool IsANumberString(const string& token);
	static bool IsANumberLiteral(const string& token);
	static bool IsADecimalNumber(const string& token);
    static bool IsAHexNumber(const string& token);

	static uint16_t Get5BitImm5(const string& token);
	static uint16_t Get9BitOffset(const string& token);
	static uint16_t Get11BitOffset(const string& token);
	static uint16_t Get6BitOffset(const string& token);

	static uint16_t ConvertStringIfNumber(const string& token);
	static uint16_t ConvertToDecimal(const string& token);
	static uint16_t ConvertRegisterStringsTo3BitAddress(const string& registerName, vector<string>& errors);
	static uint16_t ConvertFromHexToDec(const string& token);

	static uint16_t HandleLITConversion(const vector<string>& instruction);
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