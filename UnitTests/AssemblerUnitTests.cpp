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
}