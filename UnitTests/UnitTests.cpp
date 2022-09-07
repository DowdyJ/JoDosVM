#include "pch.h"
#include "CppUnitTest.h"
#include "Register.h"
#include <bitset>

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace UnitTests
{
	TEST_CLASS(UnitTests)
	{
	public:
		TEST_METHOD(AddPositiveNumberMethod1)
		{
			Register::SetValueInRegister(Register::R_R1, static_cast<uint16_t>(0));

			uint16_t instruction = 0b0001001001100100;

			Register::Add(instruction);

			Assert::AreEqual(static_cast<uint16_t>(4), Register::GetValueInReg(Register::R_R1));
		}

		TEST_METHOD(AddNegatitiveNumberMethod1)
		{
			Register::SetValueInRegister(Register::R_R1, static_cast<uint16_t>(0));


			uint16_t instruction = 0b0001001001111100;

			Register::Add(instruction);

			Assert::AreEqual(static_cast<uint16_t>(-4), Register::GetValueInReg(Register::R_R1));
		}

		TEST_METHOD(AddTwoPositiveRegisters)
		{
			//Add R1 and R2 and store the result in R1
			uint16_t instruction = 0b0001001010000001;

			Register::SetValueInRegister(Register::R_R1, static_cast<uint16_t>(5));
			Register::SetValueInRegister(Register::R_R2, static_cast<uint16_t>(1));

			Register::Add(instruction);

			Assert::AreEqual(static_cast<uint16_t>(6), Register::GetValueInReg(Register::R_R1));
		}

		TEST_METHOD(AddTwoNegativeRegisters)
		{
			//Add R1 and R2 and store the result in R1
			uint16_t instruction = 0b0001001010000001;

			Register::SetValueInRegister(Register::R_R1, static_cast<uint16_t>(-5));
			Register::SetValueInRegister(Register::R_R2, static_cast<uint16_t>(-1));

			Register::Add(instruction);

			Assert::AreEqual(static_cast<uint16_t>(-6), Register::GetValueInReg(Register::R_R1));
		}

		TEST_METHOD(AddOnePositiveOneNegativeRegister)
		{
			//Add R1 and R2 and store the result in R1
			uint16_t instruction = 0b0001001010000001;

			Register::SetValueInRegister(Register::R_R1, static_cast<uint16_t>(5));
			Register::SetValueInRegister(Register::R_R2, static_cast<uint16_t>(-1));

			Register::Add(instruction);

			Assert::AreEqual(static_cast<uint16_t>(4), Register::GetValueInReg(Register::R_R1));
		}

		TEST_METHOD(AndPositiveNumberMethod1)
		{
			Register::SetValueInRegister(Register::R_R1, static_cast<uint16_t>(1));

			uint16_t instruction = 0b0101001001100100;

			Register::And(instruction);

			Assert::AreEqual(static_cast<uint16_t>(0), Register::GetValueInReg(Register::R_R1));
		}

		TEST_METHOD(AndNegatitiveNumberMethod1)
		{
			Register::SetValueInRegister(Register::R_R1, static_cast<uint16_t>(3));


			uint16_t instruction = 0b0001001001111100;

			Register::And(instruction);

			Assert::AreEqual(static_cast<uint16_t>(0), Register::GetValueInReg(Register::R_R1));
		}

		TEST_METHOD(AndTwoPositiveRegisters)
		{
			uint16_t instruction = 0b0001001010000001;

			Register::SetValueInRegister(Register::R_R1, static_cast<uint16_t>(5));
			Register::SetValueInRegister(Register::R_R2, static_cast<uint16_t>(1));

			Register::And(instruction);

			Assert::AreEqual(static_cast<uint16_t>(1), Register::GetValueInReg(Register::R_R1));
		}

		TEST_METHOD(AndTwoNegativeRegisters)
		{
			uint16_t instruction = 0b0001001010000001;

			Register::SetValueInRegister(Register::R_R1, static_cast<uint16_t>(-5));
			Register::SetValueInRegister(Register::R_R2, static_cast<uint16_t>(-1));

			Register::And(instruction);

			Assert::AreEqual(static_cast<uint16_t>(-5), Register::GetValueInReg(Register::R_R1));
		}

		TEST_METHOD(AndOnePositiveOneNegativeRegister)
		{
			//Add R1 and R2 and store the result in R1
			uint16_t instruction = 0b0001001010000001;

			Register::SetValueInRegister(Register::R_R1, static_cast<uint16_t>(5));
			Register::SetValueInRegister(Register::R_R2, static_cast<uint16_t>(-1));

			Register::And(instruction);

			Assert::AreEqual(static_cast<uint16_t>(5), Register::GetValueInReg(Register::R_R1));
		}
	};
}
