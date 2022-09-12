#include "Assembler.h"
#include "Utilities.h"
#include <sstream>

vector<string> Assembler::_errors;

void Assembler::AssembleIntoBinary(const vector<vector<string>>& inputTokens)
{
	for (size_t i = 0; i < inputTokens.size(); i++)
	{
		string command = Utilities::ToUpperCase(inputTokens[i][0]);		

		if (command == "ADD") { Assembler::HandleADDConversion(inputTokens[i]); }
		else if (command == "AND") { Assembler::HandleANDConversion(inputTokens[i]); }
		else if (command == "NOT") { Assembler::HandleADDConversion(inputTokens[i]); }
		else if (command[0] == 'B' && command[1] == 'R') { Assembler::HandleBRConversion(inputTokens[i]); }
		else if (command == "JMP") { Assembler::HandleJMPConversion(inputTokens[i]); }
		else if (command == "JSR") { Assembler::HandleJSRConversion(inputTokens[i]); }
		else if (command == "LD") { Assembler::HandleLDConversion(inputTokens[i]); }
		else if (command == "LDR") { Assembler::HandleLDRConversion(inputTokens[i]); }
		else if (command == "LDI") { Assembler::HandleLDIConversion(inputTokens[i]); }
		else if (command == "LEA") { Assembler::HandleLEAConversion(inputTokens[i]); }
		else if (command == "ST") { Assembler::HandleSTConversion(inputTokens[i]); }
		else if (command == "STI") { Assembler::HandleSTIConversion(inputTokens[i]); }
		else if (command == "STR") { Assembler::HandleSTRConversion(inputTokens[i]); }
		else if (command == "TRAP") { Assembler::HandleTRAPConversion(inputTokens[i]); }
		else if (command == "RES") { /*BAD OP CODE*/ }
		else if (command == "RTI") { Assembler::HandleRTIConversion(inputTokens[i]); }
		//Assembler only opcodes
		else if (command == "RET") { Assembler::HandleJMPConversion(inputTokens[i]); }
		else if (command == "JSRR") { Assembler::HandleJSRConversion(inputTokens[i]); }
		else 
		{
			//BAD OPCODE
		}

	}
}

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

bool Assembler::IsADecimalNumber(const string& token)
{
	if (token[0] == '#')
		return true;
		
	return false;
}

uint16_t Assembler::Get5BitImm5(const string& token)
{
	int imm5Value = ConvertToDecimal(token, _errors);

	if (imm5Value > 15 || imm5Value < -16)
	{
		_errors.push_back("Value for imm5 was out of range of allowed values. Value was: " + imm5Value);
	}

	return static_cast<uint16_t>(imm5Value) & 0x1f;

}

int Assembler::ConvertToDecimal(const string& token, vector<string>& errors)
{
	if (token.find('#') != string::npos)
	{
		if (token.size() < 2)
		{
			errors.push_back("ConvertToDecimal failed on token " + token + " Reason: Too short");
			return 0;
		}

		return static_cast<int>(std::stoi(token.substr(1)));
	}
	else 
	{
		errors.push_back("ConvertToDecimal failed on token " + token + " Reason: did not start with # character");
		return 0;
	}
}

uint16_t Assembler::ConvertRegisterStringsTo3BitAddress(const string& registerName, vector<string>& errors)
{
	if (registerName.size() != 2 || (registerName[1] < 48 || registerName[1] > 57))
	{
		errors.push_back("Invalid register name: " + registerName + " was used in ConvertRegisterStringsTo3BitAddress.");
		return 0;
	}
	unsigned char numberAsChar = registerName[1] - 48;
	uint16_t registerIndex = static_cast<uint16_t>(numberAsChar);

	return registerIndex;
}

uint16_t Assembler::HandleADDConversion(const vector<string>& instruction)
{
	if (instruction.size() != 4)
	{
		_errors.push_back("Incorrect number of tokens for ADD. Recieved " + std::to_string(instruction.size()) + ", expected 4");
	}
	if (instruction.size() < 4)
		return 0;

	uint16_t baseInstruction = 0b0001000000000000;

	uint16_t dr = ConvertRegisterStringsTo3BitAddress(instruction[1], _errors) << 9;
	uint16_t sr1 = ConvertRegisterStringsTo3BitAddress(instruction[2], _errors) << 6;
	uint16_t mode = 0;
	uint16_t sr2 = 0;
	uint16_t imm5 = 0;

	if (IsADecimalNumber(instruction[3]))
	{
		mode = 0x20;
		imm5 = Get5BitImm5(instruction[3]);
	}
	else 
	{
		sr2 = ConvertRegisterStringsTo3BitAddress(instruction[3], _errors);
	}

	return baseInstruction | dr | sr1 | sr2 | imm5 | mode;
}

uint16_t Assembler::HandleANDConversion(const vector<string>& instruction)
{
	if (instruction.size() != 4)
	{
		_errors.push_back("Incorrect number of tokens for AND. Recieved " + std::to_string(instruction.size()) + ", expected 4");
	}

	uint16_t baseInstruction = 0b0101000000000000;

	uint16_t dr = ConvertRegisterStringsTo3BitAddress(instruction[1], _errors) << 9;
	uint16_t sr1 = ConvertRegisterStringsTo3BitAddress(instruction[2], _errors) << 6;
	uint16_t mode = 0;
	uint16_t sr2 = 0;
	uint16_t imm5 = 0;

	if (IsADecimalNumber(instruction[3]))
	{
		mode = 0x20;
		imm5 = Get5BitImm5(instruction[3]);
	}
	else
	{
		sr2 = ConvertRegisterStringsTo3BitAddress(instruction[3], _errors);
	}

	return baseInstruction | dr | sr1 | sr2 | imm5 | mode;
}

uint16_t Assembler::HandleNOTConversion(const vector<string>& instruction)
{
	if (instruction.size() != 3)
	{
		_errors.push_back("Incorrect number of tokens for NOT. Recieved " + std::to_string(instruction.size()) + ", expected 3");
	}
	if (instruction.size() < 3)
		return 0;

	uint16_t baseInstruction = 0b1001000001111111;

	uint16_t dr = ConvertRegisterStringsTo3BitAddress(instruction[1], _errors) << 9;
	uint16_t sr = ConvertRegisterStringsTo3BitAddress(instruction[2], _errors) << 6;


	return baseInstruction | dr | sr;
}

uint16_t Assembler::HandleBRConversion(const vector<string>& instruction)
{
	return uint16_t();
}

uint16_t Assembler::HandleJMPConversion(const vector<string>& instruction)
{
	uint16_t baseInstruction = 0xC000;

	if (instruction[0] == "JMP") 
	{
		if (instruction.size() < 2)
		{
			_errors.push_back("Incorrect number of tokens for JMP. Recieved " + std::to_string(instruction.size()) + ", expected 2");
			return 0;
		}
		else if (instruction.size() > 2) 
		{
			_errors.push_back("Incorrect number of tokens for JMP. Recieved " + std::to_string(instruction.size()) + ", expected 2");
		}

		uint16_t registerIndex = ConvertRegisterStringsTo3BitAddress(instruction[1], _errors);
		return baseInstruction | (registerIndex << 6);
	}
	else if (instruction[0] == "RET") 
	{
		if (instruction.size() > 1)
		{
			_errors.push_back("Incorrect number of tokens for RET. Recieved " + std::to_string(instruction.size()) + ", expected 1");
		}
		return baseInstruction | 0x1C0;
	}

	return 0;
}

uint16_t Assembler::HandleJSRConversion(const vector<string>& instruction)
{
	return uint16_t();
}

uint16_t Assembler::HandleLDConversion(const vector<string>& instruction)
{
	uint16_t baseInstruction = 0x2000;

	if (instruction.size() != 3) 
	{
		_errors.push_back("Incorrect number of tokens for LD. Recieved " + std::to_string(instruction.size()) + ", expected 3");
	}
	if (instruction.size() < 3)
		return 0;

	uint16_t dr = ConvertRegisterStringsTo3BitAddress(instruction[1], _errors) << 9;
	uint16_t pcOffset9 = 0;

	//Make labels work;
	return 0;
}

uint16_t Assembler::HandleLDIConversion(const vector<string>& instruction)
{
	return uint16_t();
}

uint16_t Assembler::HandleLDRConversion(const vector<string>& instruction)
{
	return uint16_t();
}

uint16_t Assembler::HandleLEAConversion(const vector<string>& instruction)
{
	return uint16_t();
}

uint16_t Assembler::HandleSTConversion(const vector<string>& instruction)
{
	return uint16_t();
}

uint16_t Assembler::HandleSTIConversion(const vector<string>& instruction)
{
	return uint16_t();
}

uint16_t Assembler::HandleSTRConversion(const vector<string>& instruction)
{
	return uint16_t();
}

uint16_t Assembler::HandleTRAPConversion(const vector<string>& instruction)
{
	//Takes hex in form xFF
	if (instruction.size() != 2)
	{
		_errors.push_back("Incorrect number of tokens for TRAP. Recieved " + std::to_string(instruction.size()) + ", expected 2");
	}
	if (instruction.size() < 2)
		return 0;

	if (instruction[1].size() != 3) 
	{
		_errors.push_back("Incorrect number of characters for TRAP's first argument. Recieved " + std::to_string(instruction[1].size()) + ", expected 3");
		return 0;
	}

	string trapVectorLocationHex = instruction[1].substr(1);

	uint16_t baseInstruction = 0xF000;
	uint16_t trapVector = 0;
	std::istringstream(trapVectorLocationHex) >> std::hex >> trapVector >> std::dec;

	return baseInstruction | trapVector;
}

uint16_t Assembler::HandleRTIConversion(const vector<string>& instruction) 
{
	return 0x8000;
}
