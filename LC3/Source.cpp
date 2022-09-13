#include <cstdint>
#include <iostream>
#include "Register.h"

int main(int argc, char* argv[])
{
	Register::SetValueInRegister(Register::R_PC, 0x3000);

}
