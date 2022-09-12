#include "pch.h"
#include "CppUnitTest.h"
#include "Register.h"
#include <bitset>
#include "Assembler.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTests
{
	TEST_CLASS(AssemblerUnitTests)
	{
	public:
		TEST_METHOD(RemoveCommentsNormalCase)
		{
			string testString = "Test string!; now with a comment!";

			Assembler::RemoveCommentsFromLine(testString);

			Assert::AreEqual(static_cast<string>("Test string!"), testString);
		}
		
		TEST_METHOD(RemoveCommentsMoreThanOneSemicolon)
		{
			string testString = "Test string!; ;now with a;;; comment!";

			Assembler::RemoveCommentsFromLine(testString);

			Assert::AreEqual(static_cast<string>("Test string!"), testString);
		}

		TEST_METHOD(RemoveCommentsNoSemicolon)
		{
			string testString = "Test string!";

			Assembler::RemoveCommentsFromLine(testString);

			Assert::AreEqual(static_cast<string>("Test string!"), testString);
		}

		TEST_METHOD(RemoveAllLinesAfterENDMacroOneEND) 
		{
			vector<string> testInstructions = {"TEST", "OP", "COMMANDS", "THAT", "END", "WITH", ".END", "AND SHOULDNT SHOW THIS!", "AND THIS", ";.END", ".END"};
			vector<string> testInstructionsResult = {"TEST", "OP", "COMMANDS", "THAT", "END", "WITH"};

			Assembler::RemoveAllTextAfterAndIncludingENDMacro(testInstructions);

			Assert::AreEqual(testInstructions.size(), testInstructionsResult.size());

			for (size_t i = 0; i < testInstructions.size(); ++i)
			{
				Assert::AreEqual(testInstructions[i], testInstructionsResult[i]);
			}
				
		}
	};

	TEST_CLASS(OpCodeUnitTests)
	{
	public:
		TEST_METHOD(AddModeOne)
		{
			vector<string> instructions = {"ADD","R1","R2","#4"};
			uint16_t expectedResult = 0b0001001010100100;
			uint16_t actualResult = Assembler::HandleADDConversion(instructions);

			Assert::AreEqual(expectedResult, actualResult);
		}

		TEST_METHOD(AddModeZero)
		{
			vector<string> instructions = { "ADD","R1","R2","R3" };
			uint16_t expectedResult = 0b0001001010000011;
			uint16_t actualResult = Assembler::HandleADDConversion(instructions);

			Assert::AreEqual(expectedResult, actualResult);
		}

		TEST_METHOD(AddModeOneNegativeNumber)
		{
			vector<string> instructions = { "ADD","R1","R2","#-3" };
			uint16_t expectedResult = 0b0001001010111101;
			uint16_t actualResult = Assembler::HandleADDConversion(instructions);

			Assert::AreEqual(expectedResult, actualResult);
		}

		TEST_METHOD(JumpTest)
		{
			vector<string> instructions = { "JMP", "R1" };
			uint16_t expectedResult = 0b1100000001000000;
			uint16_t actualResult = Assembler::HandleJMPConversion(instructions);

			Assert::AreEqual(expectedResult, actualResult);
		}

		TEST_METHOD(RetTest)
		{
			vector<string> instructions = { "RET" };
			uint16_t expectedResult = 0b1100000111000000;
			uint16_t actualResult = Assembler::HandleJMPConversion(instructions);

			Assert::AreEqual(expectedResult, actualResult);
		}

		TEST_METHOD(TrapTest)
		{
			vector<string> instructions = { "TRAP", "xFF"};
			uint16_t expectedResult = 0xF0FF;
			uint16_t actualResult = Assembler::HandleTRAPConversion(instructions);

			Assert::AreEqual(expectedResult, actualResult);
		}
	};



}