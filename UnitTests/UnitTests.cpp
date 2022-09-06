#include "pch.h"
#include "CppUnitTest.h"
#include "Register.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTests
{
	TEST_CLASS(UnitTests)
	{
	public:
		
		TEST_METHOD(TestMethod1)
		{
			uint16_t instruction = 0b0001001001100000;
			uint16_t numberToAdd = 0b0000000000000100;
			instruction |= numberToAdd;
			Register::Add(instruction);

			Assert::AreEqual(static_cast<uint16_t>(4), Register::GetValueInReg(Register::R_R1));
			Assert::AreNotEqual(static_cast<uint16_t>(5), Register::GetValueInReg(Register::R_R1));
		}
	};
}
