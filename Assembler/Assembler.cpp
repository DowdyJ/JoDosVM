#include "Assembler.h"
#include "Utilities.h"


vector<string> Assembler::_errors;
//map<string, uint16_t> Assembler::labelIndexPairs;


//Assumes all labels have been converted to 16 bit offsets in decimal form without pound sign
vector<uint16_t> Assembler::AssembleIntoBinary(const vector<vector<string>>& inputTokens1)
{
	vector<vector<string>> inputTokens = inputTokens1;
	vector<uint16_t> output;
	
	for (size_t i = 0; i < inputTokens.size(); i++)
	{
		string command = Utilities::ToUpperCase(inputTokens[i][0]);		

		//Custom opcode inserted by macro processing. Resulting "instruction is" LITeral value of the operand.
		if (command == "LIT") 
		{
			output.push_back(Assembler::HandleLITConversion(inputTokens[i]));
			continue;
		}


		if (command == "ADD") { output.push_back(Assembler::HandleADDConversion(inputTokens[i])); }
		else if (command == "AND") { output.push_back(Assembler::HandleANDConversion(inputTokens[i])); }
		else if (command == "NOT") { output.push_back(Assembler::HandleNOTConversion(inputTokens[i])); }
		else if (
			command == "BR" || 
			command == "BRN" || 
			command == "BRZ" || 
			command == "BRP" || 
			command == "BRZP" ||
			command == "BRNP" ||
			command == "BRNZ" ||
			command == "BRNZP") { output.push_back(Assembler::HandleBRConversion(inputTokens[i])); }
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
		else if (command == "RES") { _errors.push_back("Use of illegal OpCode RES!"); }
		else if (command == "RTI") { output.push_back(Assembler::HandleRTIConversion(inputTokens[i])); }
		//Assembler only opcodes
		else if (command == "RET") { output.push_back(Assembler::HandleJMPConversion(inputTokens[i])); }
		else if (command == "JSRR") { output.push_back(Assembler::HandleJSRConversion(inputTokens[i])); }
		else 
		{
			Assembler::_errors.push_back("Unexpected OpCode enctountered on line " + std::to_string(i) + ". OpCode was: " + command);
		}

	}

	return output;
}


void Assembler::ResolveAndReplaceLabels(vector<vector<string>>& inputTokens)
{
	vector<string> opCodesRequiringLabelChecks = 
	{ "BR", "BRP", "BRN", "BRZ", "BRZP", "BRNP", "BRNZ", "BRNZP",
	"JSR", "LD", "LDI", "LEA", "ST", "STI"};

	map<string, uint16_t> labelIndexPairs = Assembler::BuildLabelAddressMap(inputTokens, Assembler::_errors);

	Assembler::ReplaceLabelsWithOffsets(inputTokens, opCodesRequiringLabelChecks, labelIndexPairs, Assembler::_errors);

}


void Assembler::HandleORIGMacro(vector<string>& linifiedFile) 
{
	if (linifiedFile[0].find(".ORIG") != 0)
	{
		//Error! All files need to start with this.
		return;
	}

	string valueAtIndex = linifiedFile[0];
	size_t indexOfX = valueAtIndex.find('x');
	string hexValue = valueAtIndex.substr(indexOfX + 1);

	uint16_t valueAsInt = 0;

	std::stringstream ss;

	ss << std::hex << hexValue;
	ss >> valueAsInt;
	
	linifiedFile[0] = "LIT " + std::to_string(valueAsInt);

	return;
}

void Assembler::HandleFILLMacros(vector<vector<string>>& tokeninzedInput) 
{
	// Expects input in the form .FILL x[0-9a-f]+ or label

	for (size_t i = 0; i < tokeninzedInput.size(); ++i)
	{
		vector<string> currentLine = tokeninzedInput[i];
		
		for (size_t j = 0; j < currentLine.size(); ++j)
		{
			if (Utilities::ToUpperCase(currentLine[j]) == ".FILL")
			{
				if (j + 1 >= currentLine.size())
				{
					std::cout << "ERROR: .FILL macro was missing required arguments. Replacing with NOP" << std::endl;
					tokeninzedInput[i] = {"LIT", "0"};
					return;
				}

				uint16_t valueAsInt;
				bool isNumberArg = false;
				string fillArgumentString = currentLine[j + 1];

				if (IsANumberString(fillArgumentString))
					valueAsInt = ConvertStringIfNumber(fillArgumentString);
				else
					isNumberArg = false;

				if (j != 0)
				{
					string label = currentLine[0];
					if (!isNumberArg)
						tokeninzedInput[i] = {label, "LIT", fillArgumentString};
					else 
						tokeninzedInput[i] = {label, "LIT", std::to_string(valueAsInt)};
				} 
				else
				{
					if (!isNumberArg)
						tokeninzedInput[i] = {"LIT", fillArgumentString};
					else 
						tokeninzedInput[i] = {"LIT", std::to_string(valueAsInt)};

				}
			}
		}
	}
}

void Assembler::HandleTRAPCodeMacroReplacement(vector<vector<string>>& tokeninzedInput) 
{
	std::map<string, vector<string>> labelToTrapCode = 
	{
		{"GETC",  vector<string>{"TRAP", "x20"}},
		{"OUT",   vector<string>{"TRAP", "x21"}},
		{"PUTS",  vector<string>{"TRAP", "x22"}},
		{"IN",    vector<string>{"TRAP", "x23"}},
		{"PUTSP", vector<string>{"TRAP", "x24"}},
		{"HALT",  vector<string>{"TRAP", "x25"}},
	};

	for (size_t i = 0; i < tokeninzedInput.size(); ++i)
	{
		vector<string> currentLine = tokeninzedInput[i];

		for (size_t j = 0; j < currentLine.size(); ++j)
		{
			string currentToken = Utilities::ToUpperCase(currentLine[j]);
			if (labelToTrapCode.find(currentToken) != labelToTrapCode.end())
			{
				//replace NAME with TRAP x##
				vector<string> trapCodeVector = labelToTrapCode[currentToken];
				tokeninzedInput[i].erase(tokeninzedInput[i].begin() + j, tokeninzedInput[i].end());
				tokeninzedInput[i].insert(tokeninzedInput[i].begin() + j, trapCodeVector.begin(), trapCodeVector.end());
			}
		}
	}	
}

void Assembler::HandleSTRINGZMacros(vector<vector<string>>& tokeninzedInput) 
{
	for (size_t i = 0; i < tokeninzedInput.size(); ++i)
	{
		vector<string> currentLine = tokeninzedInput[i];

		for (size_t j = 0; j < currentLine.size(); ++j)
		{
			string currentToken = Utilities::ToUpperCase(currentLine[j]);
			if (currentToken == ".STRINGZ")
			{
				if (j + 1 > currentLine.size())
				{
					std::cout << "ERROR: .STRINGZ lacked it's required argument! Replacing with NOP" << std::endl;
					tokeninzedInput[i] = vector<string> {"LIT 0"};
					return;
				}


				string stringzArgument = Utilities::ConcatenateStrings(vector<string>(currentLine.begin() + j + 1, currentLine.end()));
				vector<vector<string>> newInstructions;

				if (j > 0) // Add the label if there was one
					newInstructions.push_back(vector<string>{currentLine[0]});	

				for (size_t i = 1; i < stringzArgument.size() - 1; ++i) // Start at one and end one early removes quote marks.
				{
					char c = stringzArgument[i];
					if (c == '\\' && i + 1 < stringzArgument.size())
					{
						char nextChar = stringzArgument[i+1];
						if (nextChar == 'n' || nextChar == 'N') // LF
						{
							newInstructions.push_back(vector<string>{"LIT", std::to_string(10)});
							++i;
							continue;
						} 
						else if (nextChar == 'e' || nextChar == 'E') // ESC
						{
							newInstructions.push_back(vector<string>{"LIT", std::to_string(27)});
							++i;
							continue;
						}
						else // just a loose backslash or unhandled escape sequence
						{
							newInstructions.push_back(vector<string> {"LIT", std::to_string(static_cast<uint16_t>(c))});
						}
					} 
					else 
						newInstructions.push_back(vector<string> {"LIT", std::to_string(static_cast<uint16_t>(c))});
				}

				newInstructions.push_back(vector<string>{"LIT", "0"}); // Add null terminator
				tokeninzedInput.erase(tokeninzedInput.begin() + i); // Remove original stringz macro command
				tokeninzedInput.insert(tokeninzedInput.begin() + i, newInstructions.begin(), newInstructions.end());

				i += newInstructions.size() - 1;
			}
		}
	}	

	return;
}

void Assembler::ReplaceLabelsWithOffsets(vector<vector<string>>& inputTokens, const vector<string>& opCodesToCheck, const map<string, uint16_t>& labelIndexPairs, vector<string>& errors) 
{
	for (size_t i = 0; i < inputTokens.size(); ++i)
	{
		for (size_t j = 0; j < opCodesToCheck.size(); j++)
		{
			string opcode = Utilities::ToUpperCase(inputTokens[i][0]);
			
			if (opcode == opCodesToCheck[j])
			{
				string label;

				if (opcode[0] == 'B' || opcode[0] == 'J') //BR(nzp) and JSR
				{
					label = Utilities::ToUpperCase(inputTokens[i][1]);

					if (labelIndexPairs.find(label) != labelIndexPairs.end())
					{
						uint16_t labelLineNumber = labelIndexPairs.at(label);
						
						inputTokens[i][1] = std::to_string(static_cast<int>(labelLineNumber) - static_cast<int>(i + 1)); //Add 1 to the line number because the offsets are relative to the INCREMENTED PC.
					}
					else 
					{
						if (label[0] != '#' && label[0] != 'x' && label[0] != 'X')
							errors.push_back("Unregistered label: " + label  + " used in line " + std::to_string(i) + ". Full line is as follows: " + Utilities::ConcatenateStrings(inputTokens[i]));
					}
				}
				else 
				{
					label = Utilities::ToUpperCase(inputTokens[i][2]);

					if (labelIndexPairs.find(label) != labelIndexPairs.end())
					{
						uint16_t labelLineNumber = labelIndexPairs.at(label);

						inputTokens[i][2] = std::to_string(static_cast<int>(labelLineNumber) - static_cast<int>(i + 1));
					}
					else 
					{
						if (label[0] != '#' && label[0] != 'x' && label[0] != 'X')
							errors.push_back("Unregistered label: " + label + " used in line " + std::to_string(i));
					}
				}

				break;
			}
		}
	}
}

map<string, uint16_t> Assembler::BuildLabelAddressMap(vector<vector<string>>& inputTokens, vector<string>& errors) 
{
	vector<string> opCodes = { "ADD", "AND", "JMP", "RET", "JSR", "JSRR", "LD", "LDI", "LDR", "LEA", "NOT", "ST", "STI", "STR", "TRAP", "RTI",
	"BR", "BRP", "BRN", "BRZ", "BRZP", "BRNP", "BRNZ", "BRNZP", "LIT" };


	map<string, uint16_t> labelIndexPairs;

	for (size_t i = 0; i < inputTokens.size(); ++i)
	{
		vector<string> lineOfTokens = inputTokens[i];
		
		string label = Utilities::ToUpperCase(lineOfTokens[0]);
			
		if (std::find(opCodes.begin(), opCodes.end(), label) == opCodes.end()) // It is not an OP Code
		{
			if (labelIndexPairs.find(label) == labelIndexPairs.end()) // It isn't in the label map yet
			{
				if (lineOfTokens.size() > 1)
				{
					labelIndexPairs.insert({ label, i });
					inputTokens[i].erase(inputTokens[i].begin());
				}
				else
				{
					labelIndexPairs.insert({ label, i });
					inputTokens.erase(inputTokens.begin() + i);
				}
				--i;
			}
			else
			{
				errors.push_back("Duplicate label in input: " + label);
			}
		}
		
	}

	std::cout << "-----------------------------" << std::endl;
	std::cout << "Found the following labels: " << std::endl;
	if (labelIndexPairs.size() > 0)
	{
		for (auto itr = labelIndexPairs.begin(); itr != labelIndexPairs.end(); ++itr)
		{
			std::cout << itr->first << " line " << itr->second << std::endl;
		}
	}

	return labelIndexPairs;
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

		bool isStringZArgumentToken = false;

		for (size_t characterIndex = 0; characterIndex < currentLineOfInput.size(); ++characterIndex)
		{	
			if (currentLineOfInput[characterIndex] == ' ' || currentLineOfInput[characterIndex] == ',' || currentLineOfInput[characterIndex] == '\t')
				continue;

			string currentToken = "";

			if (isStringZArgumentToken) 
			{
				size_t closingQuoteIndex = currentLineOfInput.find_last_of('"');

				if (closingQuoteIndex == characterIndex)
					_errors.push_back("Ending quote not found for STRINGZ argument. Undesired behavior may occur.");

				currentToken = currentLineOfInput.substr(characterIndex, closingQuoteIndex - characterIndex + 1);
				
				tokenizedLine.push_back(currentToken);
				
				characterIndex += currentToken.size();
				isStringZArgumentToken = false;
				continue;
			}


			for (;
				characterIndex < currentLineOfInput.size() && currentLineOfInput[characterIndex] != ' ' && currentLineOfInput[characterIndex] != ',' && currentLineOfInput[characterIndex] != '\t';
				++characterIndex)
			{
				currentToken += currentLineOfInput[characterIndex];
			}

			if (Utilities::ToUpperCase(currentToken) == ".STRINGZ")
				isStringZArgumentToken = true;

			tokenizedLine.push_back(currentToken);
		}

		tokenizedInput.push_back(tokenizedLine);
	}

	return tokenizedInput;
}

void Assembler::RemoveCommentsFromLine(string& inputLine)
{
	size_t indexOfSemicolon = inputLine.find(';');

	if (indexOfSemicolon == std::string::npos)
	return;

	for (size_t i = 0; i < indexOfSemicolon; ++i)
	{
		char currentChar = inputLine[indexOfSemicolon - i];
		if (currentChar == ' ')
			break;
		else if (currentChar == '\\' && 
		(inputLine[indexOfSemicolon - i + 1] == 'e' || inputLine[indexOfSemicolon - i + 1] == 'E'))
		{
			indexOfSemicolon = inputLine.find(';', indexOfSemicolon + 1);
			
			if (indexOfSemicolon == std::string::npos)
				return;

			i = 0;
		}
	}
	inputLine = inputLine.substr(0, indexOfSemicolon);
	return;
}

void Assembler::HandleENDMacro(vector<string>& linifiedFile)
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

bool Assembler::IsANumberString(const string& token) 
{
	if (IsANumberLiteral(token) || IsAHexNumber(token) || IsADecimalNumber(token))
		return true;

	return false;
}

uint16_t Assembler::ConvertStringIfNumber(const string& token) 
{
	try
	{
		if (IsANumberLiteral(token))
		{
			return static_cast<uint16_t>(std::stoi(token));
		} 
		else if (IsAHexNumber(token)) 
		{
			return ConvertFromHexToDec(token);
		}
		else if (IsADecimalNumber(token))
		{
			return ConvertToDecimal(token);
		}
		else 
		{
			_errors.push_back("Tried to convert the token: " + token + " to integer but format was not recognized.");
		}

	}
	catch(const std::invalid_argument& e)
	{
		std::cout << "Exception in ConvertStringIfNumber on token: " << token << ". Tried to convert to int." << std::endl;
		std::cerr << e.what() << '\n';
	}
	
	return 0;
}

bool Assembler::IsANumberLiteral(const string& token) 
{
	for (char const& c : token)
	{
		if ((c < 48 || c > 57) && c != 0 && c != 45) // Not ASCII numeral, '-', or null terminator
			return false;
	}

	return true;
}

bool Assembler::IsADecimalNumber(const string& token)
{
	if (token[0] == '#')
		return true;
		
	return false;
}

bool Assembler::IsAHexNumber(const string& token)
{
	if (token[0] == 'x' || token[0] == 'X')
		return true;
	
	return false;
}

uint16_t Assembler::Get5BitImm5(const string& token)
{
	uint16_t imm5Value;

	if (IsANumberString(token))
		imm5Value = ConvertStringIfNumber(token);
	else
		_errors.push_back("Input for Get5BitImm5() was not a valid format. Recieved: " + token);

	imm5Value &= 0x1f;

	return imm5Value;

}

uint16_t Assembler::Get6BitOffset(const string& token)
{
	uint16_t imm6Value;

	if (IsANumberString(token))
		imm6Value = ConvertStringIfNumber(token);
	else
		_errors.push_back("Input for Get6BitOffset() was not a valid format. Recieved: " + token);


	imm6Value &= 0x3f;

	return imm6Value;

}

uint16_t Assembler::Get9BitOffset(const string& token) 
{
	uint16_t rawOffset; 

	if (IsANumberString(token))
		rawOffset = ConvertStringIfNumber(token);
	else
		_errors.push_back("Input for Get9BitOffset() was not a valid format. Recieved: " + token);

	
	rawOffset &= 0x1ff;

	return rawOffset;
	
}


uint16_t Assembler::ConvertFromHexToDec(const string& token) 
{
	string hexValueString = token.substr(1);
	uint16_t decimalVal = 0;
	std::istringstream(hexValueString) >> std::hex >> decimalVal >> std::dec;

	return decimalVal;
}

uint16_t Assembler::Get11BitOffset(const string& token)
{
	uint16_t rawOffset;

	if (IsANumberString(token))
		rawOffset = ConvertStringIfNumber(token);
	else
		_errors.push_back("Input for Get11BitOffset() was not a valid format. Recieved: " + token);

	rawOffset &= 0x7FF;

	return rawOffset;

}

uint16_t Assembler::ConvertToDecimal(const string& token)
{
	if (token[0] == '#')
	{
		if (token.size() < 2)
		{
			_errors.push_back("ConvertToDecimal failed on token " + token + " Reason: Too short");
			return 0;
		}
		
		return static_cast<uint16_t>(std::stoi(token.substr(1)));
	} 
	else 
	{
		_errors.push_back("ConvertToDecimal failed on token " + token + " Reason: did not start with '#' character");
		return 0;
	}
}

uint16_t Assembler::ConvertRegisterStringsTo3BitAddress(const string& registerName, vector<string>& errors)
{
	unsigned char numberAsChar;

	if (registerName[0] == 'x' || registerName[0] == 'X')
	{
		numberAsChar = static_cast<char>(Assembler::ConvertFromHexToDec(registerName));
	}
	else if (registerName.size() != 2 || (registerName[1] < 48 || registerName[1] > 57))
	{
		errors.push_back("Invalid register name: " + registerName + " was used in ConvertRegisterStringsTo3BitAddress.");
		return 0;
	} else 
	{
		numberAsChar = registerName[1] - 48;
	}

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

uint16_t Assembler::HandleLITConversion(const vector<string>& instruction) 
{
	// instruction should be of the format "LIT [0-9]+"
	string valueOfLitArgAsString = instruction[1];
	uint16_t valueOfArgAsInt;

	if (IsANumberString(valueOfLitArgAsString))
		valueOfArgAsInt = ConvertStringIfNumber(valueOfLitArgAsString);
	else 
		_errors.push_back("Value for LIT was not a number. Recieved: " + valueOfLitArgAsString);
	
	return valueOfArgAsInt;
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

	if (IsANumberString(instruction[3]))
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

	uint16_t baseInstruction = 0b1001000000111111;

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
	
	uint16_t pcOffset9;

	pcOffset9 = Get9BitOffset(instruction[1]);

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

			if (letter == 'N' || letter == 'n')
				flags |= 0x4;
			if (letter == 'Z' || letter == 'z')
				flags |= 0x2;
			if (letter == 'P' || letter == 'p')
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
		pcOffset11 = Get11BitOffset(instruction[1]);
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
	uint16_t pcOffset9 = Get9BitOffset(instruction[2]);

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
	uint16_t pcOffset9 = Get9BitOffset(instruction[2]);

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
	uint16_t offset6 = Get6BitOffset(instruction[3]);

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
	uint16_t pcOffset9 = Get9BitOffset(instruction[2]);

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
	uint16_t pcOffset9 = Get9BitOffset(instruction[2]);

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
	uint16_t pcOffset9 = Get9BitOffset(instruction[2]);

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
	uint16_t offset6 = Get6BitOffset(instruction[3]);

	return baseInstruction | dr | baseR | offset6;
}

uint16_t Assembler::HandleTRAPConversion(const vector<string>& instruction)
{
	//Takes hex in form XFF
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
