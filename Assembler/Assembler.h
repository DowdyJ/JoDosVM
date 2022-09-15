#pragma once
#include <map>
#include <string>
#include <vector>
#include "Logger.h"

#define private public

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

	static void RemoveAllTextAfterAndIncludingENDMacro(vector<string>& linifiedFile);

	//not implemented
	static void RemoveExcessWhiteSpaceFromLine(string& inputLine);

	void AddError(const string& errorMessage);

	static void LogErrors(Logger& logger);

	static bool AreErrors();

private:
	static vector<string> _errors;

	static map<string, uint16_t> BuildLabelAddressMap(vector<vector<string>>& inputTokens, vector<string>& errors);
	static void ReplaceLabelsWithOffsets(vector<vector<string>>& inputTokens, const vector<string>& opCodesToCheck, const map<string, uint16_t>& labelIndexPairs, vector<string>& errors);
	//static map<string, uint16_t> labelIndexPairs;

	static bool IsADecimalNumber(const string& token);

	static uint16_t Get5BitImm5FromDecimal(const string& token);
	//"pure" decimals are literal numbers e.g. 5, 7, 12. They are from converted labels. Other decimals are in assembly format e.g. #7, #8
	static uint16_t Get9BitPCOffsetFromPureDecimal(const string& token);
	static uint16_t Get11BitPCOffsetFromPureDecimal(const string& token);
	static uint16_t Get6BitOffsetFromDecimal(const string& token);
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