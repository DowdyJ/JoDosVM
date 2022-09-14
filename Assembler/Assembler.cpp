#include "Assembler.h"
#include "Utilities.h"
#include <sstream>

vector<string> Assembler::_errors;


//Assumes all labels have been converted to 16 bit offsets in decimal form without pound sign
vector<uint16_t> Assembler::AssembleIntoBinary(const vector<vector<string>>& inputTokens)
{
	vector<uint16_t> output;

	for (size_t i = 0; i < inputTokens.size(); i++)
	{
		string command = Utilities::ToUpperCase(inputTokens[i][0]);		

		if (command == "ADD") { output.push_back(Assembler::HandleADDConversion(inputTokens[i])); }
		else if (command == "AND") { output.push_back(Assembler::HandleANDConversion(inputTokens[i])); }
		else if (command == "NOT") { output.push_back(Assembler::HandleADDConversion(inputTokens[i])); }
		else if (command[0] == 'B' && command[1] == 'R') { output.push_back(Assembler::HandleBRConversion(inputTokens[i])); }
		else if (command == "JMP") { output.push_back(Assembler::HandleJMPConversion(inputTokens[i])); }
		else if (command == "JSR") { output.push_back(Assembler::HandleJSRConversion(inputTokens[i])); }
		else if (command == "LD") { output.push_back(Assembler::HandleLDConversion(inputTokens[i])); }
		else if (command == "LDR") { output.push_back(Assembler::HandleLDRConversion(inputTokens[i])); }
		else if (command == "LDI") { output.push_back(Assembler::HandleLDIConversion(inputTokens[i])); }
		else if (command == "LEA") { output.push_back(Assembler::HandleLEAConversion(inputTokens[i])); }
		else if (command == "ST") { output.push_back(Assembler::HandleSTConversion(inputTokens[i])); }
		else if (command == "STI") { output.push_back(Assembler::HandleSTIConversion(inputTokens[i])); }
		else if (command == "STR") { output.push_back(Assembler::HandleSTRConversion(inputTokens[i])); }
		else if (command == "TRAP") { output.push_back(Assembler::HandleTRAPConversion(inputTokens[i])); }
		else if (command == "RES") { /*BAD OP CODE*/ }
		else if (command == "RTI") { output.push_back(Assembler::HandleRTIConversion(inputTokens[i])); }
		//Assembler only opcodes
		else if (command == "RET") { output.push_back(Assembler::HandleJMPConversion(inputTokens[i])); }
		else if (command == "JSRR") { output.push_back(Assembler::HandleJSRConversion(inputTokens[i])); }
		else 
		{
			_errors.push_back("Unrecognized opcode \"" + command + "\"");
		}

	}

	return output;
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

bool Assembler::AreErrors() 
{
	if (_errors.size() > 0)
		return true;
	
	return false;
}

void Assembler::LogErrors(Logger& logger)
{
	logger.Log(_errors);
}

bool Assembler::IsADecimalNumber(const string& token)
{
	if (token[0] == '#')
		return true;
		
	return false;
}

uint16_t Assembler::Get5BitImm5FromDecimal(const string& token)
{
	int imm5Value = ConvertToDecimal(token, _errors);

	if (imm5Value > 15 || imm5Value < -16)
	{
		_errors.push_back("Value for imm5 was out of range of allowed values. Value was: " + imm5Value);
	}

	return static_cast<uint16_t>(imm5Value) & 0x1f;

}

uint16_t Assembler::Get6BitOffsetFromDecimal(const string& token)
{
	int imm6Value = ConvertToDecimal(token, _errors);

	if (imm6Value > 31 || imm6Value < -32)
	{
		_errors.push_back("Value for imm6 was out of range of allowed values. Value was: " + imm6Value);
	}

	return static_cast<uint16_t>(imm6Value) & 0x3f;

}

uint16_t Assembler::Get9BitPCOffsetFromPureDecimal(const string& token) 
{
	int rawOffset = std::stoi(token);

	if (rawOffset < -256 || rawOffset > 255)
	{
		_errors.push_back("pcOffset9 was out of range of allowed values. Value was: " + rawOffset);
	}

	return static_cast<uint16_t>(rawOffset) & 0x1ff;
	
}

uint16_t Assembler::Get11BitPCOffsetFromPureDecimal(const string& token)
{
	int rawOffset = std::stoi(token);

	if (rawOffset < -1024 || rawOffset > 1023)
	{
		_errors.push_back("pcOffset11 was out of range of allowed values. Value was: " + rawOffset);
	}

	return static_cast<uint16_t>(rawOffset) & 0x7ff;

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
		imm5 = Get5BitImm5FromDecimal(instruction[3]);
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
		imm5 = Get5BitImm5FromDecimal(instruction[3]);
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
	if (instruction.size() != 2)
	{
		_errors.push_back("Incorrect number of tokens for AND. Recieved " + std::to_string(instruction.size()) + ", expected 2");
		return 0;
	}

	uint16_t baseInstruction = 0b0000000000000000;

	uint16_t pcOffset9 = Get9BitPCOffsetFromPureDecimal(instruction[1]);

	uint16_t flags = 0;

	if (instruction[0] == "BR") 
	{
		flags |= 0x7;
	}
	else 
	{
		for (size_t i = 0; i < instruction[0].size(); i++)
		{
			char letter = instruction[0][i];

			if (letter == 'N')
				flags |= 0x4;
			if (letter == 'Z')
				flags |= 0x2;
			if (letter == 'P')
				flags |= 0x1;
		}
	}
	flags <<= 9;

	return baseInstruction | flags | pcOffset9;
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
	if (instruction.size() != 2)
	{
		_errors.push_back("Incorrect number of tokens for JSR/JSRR. Recieved " + std::to_string(instruction.size()) + ", expected 2");
	}
	if (instruction.size() < 2)
		return 0;


	uint16_t baseInstruction = 0x4000;
	uint16_t flag = 0;
	uint16_t baseR = 0;
	uint16_t pcOffset11 = 0;

	if (instruction[0] == "JSRR") 
	{
		baseR = ConvertRegisterStringsTo3BitAddress(instruction[1], _errors) << 6;
	}
	else //JSR 
	{
		flag = 0x800;
		pcOffset11 = Get11BitPCOffsetFromPureDecimal(instruction[1]);
	}

	return baseInstruction | flag | baseR | pcOffset11;
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
	uint16_t pcOffset9 = Get9BitPCOffsetFromPureDecimal(instruction[2]);

	return baseInstruction | dr | pcOffset9;
}

uint16_t Assembler::HandleLDIConversion(const vector<string>& instruction)
{
	uint16_t baseInstruction = 0xA000;

	if (instruction.size() != 3)
	{
		_errors.push_back("Incorrect number of tokens for LDI. Recieved " + std::to_string(instruction.size()) + ", expected 3");
	}
	if (instruction.size() < 3)
		return 0;

	uint16_t dr = ConvertRegisterStringsTo3BitAddress(instruction[1], _errors) << 9;
	uint16_t pcOffset9 = Get9BitPCOffsetFromPureDecimal(instruction[2]);

	return baseInstruction | dr | pcOffset9;
}

uint16_t Assembler::HandleLDRConversion(const vector<string>& instruction)
{
	uint16_t baseInstruction = 0x6000;

	if (instruction.size() != 4)
	{
		_errors.push_back("Incorrect number of tokens for LDR. Recieved " + std::to_string(instruction.size()) + ", expected 4");
	}
	if (instruction.size() < 4)
		return 0;

	uint16_t dr = ConvertRegisterStringsTo3BitAddress(instruction[1], _errors) << 9;
	uint16_t baseR = ConvertRegisterStringsTo3BitAddress(instruction[2], _errors) << 6;
	uint16_t offset6 = Get6BitOffsetFromDecimal(instruction[3]);

	return baseInstruction | dr | baseR | offset6;
}

uint16_t Assembler::HandleLEAConversion(const vector<string>& instruction)
{
	uint16_t baseInstruction = 0xE000;

	if (instruction.size() != 3)
	{
		_errors.push_back("Incorrect number of tokens for LEA. Recieved " + std::to_string(instruction.size()) + ", expected 3");
	}
	if (instruction.size() < 3)
		return 0;

	uint16_t dr = ConvertRegisterStringsTo3BitAddress(instruction[1], _errors) << 9;
	uint16_t pcOffset9 = Get9BitPCOffsetFromPureDecimal(instruction[2]);

	return baseInstruction | dr | pcOffset9;
}

uint16_t Assembler::HandleSTConversion(const vector<string>& instruction)
{
	uint16_t baseInstruction = 0x3000;

	if (instruction.size() != 3)
	{
		_errors.push_back("Incorrect number of tokens for ST. Recieved " + std::to_string(instruction.size()) + ", expected 3");
	}
	if (instruction.size() < 3)
		return 0;

	uint16_t sr = ConvertRegisterStringsTo3BitAddress(instruction[1], _errors) << 9;
	uint16_t pcOffset9 = Get9BitPCOffsetFromPureDecimal(instruction[2]);

	return baseInstruction | sr | pcOffset9;
}

uint16_t Assembler::HandleSTIConversion(const vector<string>& instruction)
{
	uint16_t baseInstruction = 0xB000;

	if (instruction.size() != 3)
	{
		_errors.push_back("Incorrect number of tokens for STI. Recieved " + std::to_string(instruction.size()) + ", expected 3");
	}
	if (instruction.size() < 3)
		return 0;

	uint16_t sr = ConvertRegisterStringsTo3BitAddress(instruction[1], _errors) << 9;
	uint16_t pcOffset9 = Get9BitPCOffsetFromPureDecimal(instruction[2]);

	return baseInstruction | sr | pcOffset9;
}

uint16_t Assembler::HandleSTRConversion(const vector<string>& instruction)
{
	uint16_t baseInstruction = 0x7000;

	if (instruction.size() != 4)
	{
		_errors.push_back("Incorrect number of tokens for STR. Recieved " + std::to_string(instruction.size()) + ", expected 4");
	}
	if (instruction.size() < 4)
		return 0;

	uint16_t dr = ConvertRegisterStringsTo3BitAddress(instruction[1], _errors) << 9;
	uint16_t baseR = ConvertRegisterStringsTo3BitAddress(instruction[2], _errors) << 6;
	uint16_t offset6 = Get6BitOffsetFromDecimal(instruction[3]);

	return baseInstruction | dr | baseR | offset6;
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
