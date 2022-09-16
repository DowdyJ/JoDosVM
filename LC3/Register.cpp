#include "Register.h"
#include "ExternalUtilities.h"
#include <string>
#include <iostream>
#include <exception>

Register::Register()
{
}

Register::~Register()
{
}

void Register::WriteMemoryAt(uint16_t address, uint16_t value)
{
    if (address < MEM_MAX)
        memory[address] = value;
}

uint16_t Register::reg[R_COUNT];
uint16_t Register::memory[MEM_MAX] = {0};
bool Register::shouldBeRunning = false;

void Register::UpdateFlags(REGISTER regIndex)
{
    uint16_t valueAtRegister = reg[regIndex];

    if (valueAtRegister == 0)
    {
        reg[R_COND] = FL_ZRO;
    }
    else if ((valueAtRegister >> 15) & 1) // negative bit is set
    {
        reg[R_COND] = FL_NEG;
    }
    else
    {
        reg[R_COND] = FL_POS;
    }
}



uint16_t Register::ReadMemoryAt(uint16_t address) 
{
    try
    {
        if (address > MEM_MAX - 1)
        {
            throw (address);
        }
    }
    catch (uint16_t address)
    {
        std::cout << "Attempted to read data outside of MEM_MAX. Attempted Address was: " << address << std::endl;
        return 0;
    }

    if (address == Register::MR_KBSR)
    {
        if (ExternalUtilities::check_key())
        {
            Register::memory[Register::MR_KBSR] = (1 << 15);
            Register::memory[Register::MR_KBDR] = getchar();
        }
        else 
        {
            Register::memory[Register::MR_KBSR] = 0;
        }
    }
    
    return Register::memory[address];
}


void Register::ProcessProgram()
{
    reg[R_COND] = FL_ZRO;

    enum { PC_START = 0x3000 };

    reg[R_PC] = PC_START;

    int running = 1;
    while (running)
    {
        ProcessWord();
    }
}

void Register::Add(const uint16_t& instruction)
{
    // xxxx xxx xxx x xx xxx
    // inst  dr sr1 m xx sr2
    // inst  dr sr1 m imm5
    uint16_t destinationRegister = (instruction >> 9) & 0x7;
    uint16_t firstRegister = (instruction >> 6) & 0x7;
    if ((instruction >> 5) & 1) // alt-add mode
    {
        uint16_t valueToAdd = (instruction) & 0b00000000011111;
        valueToAdd = ExtendSign(valueToAdd, 5);
    
        reg[destinationRegister] = reg[firstRegister] + valueToAdd;
    }
    else // first add mode 
    {
        uint16_t secondRegister = (instruction) & 0x7;
      
        reg[destinationRegister] = reg[firstRegister] + reg[secondRegister];
    }
    UpdateFlags(static_cast<REGISTER>(destinationRegister));
    return;
}

void Register::And(const uint16_t& instruction)
{
    // xxxx xxx xxx x xx xxx
    // inst  dr sr1 m xx sr2
    // inst  dr sr1 m imm5
    uint16_t firstRegister = (instruction >> 6) & 0x7;
    uint16_t destinationRegister = (instruction >> 9) & 0x7;

    if ((instruction >> 5) & 1) // alt-and mode
    {
        uint16_t valueToAnd = (instruction) & 0b00000000011111;
        valueToAnd = ExtendSign(valueToAnd, 5);

        reg[destinationRegister] = reg[firstRegister] & valueToAnd;
    }
    else // normal mode
    {
        uint16_t secondRegister = (instruction) & 0x7;
        reg[destinationRegister] = reg[firstRegister] & reg[secondRegister];
    }

    UpdateFlags(static_cast<REGISTER>(destinationRegister));
    return;
    
}

void Register::Not(const uint16_t& instruction) 
{
    // xxxx xxx xxx x xxxxx
    // inst DR  SR  x xxxxx
    uint16_t destinationRegister = (instruction >> 9) & 0x7;
    uint16_t firstRegister = (instruction >> 6) & 0x7;

    reg[destinationRegister] = ~reg[firstRegister];

    Register::UpdateFlags(static_cast<REGISTER>(destinationRegister));
}

void Register::Jmp(const uint16_t& instruction) 
{
    // xxxx xxx xxx xxxxxx
    // inst xxx reg xxxxxx

    uint16_t regIndex = (instruction >> 6) & 0x7;
    reg[R_PC] = reg[regIndex];

    return;
}

void Register::Jsr(const uint16_t& instruction) 
{
    // xxxx x xxxxxxxxxxx JSR
    // inst m1PCOffset11
    // xxxx x xx xxx xxxxxx
    // inst m0xx reg xxxxxx

    bool flagSet = (instruction >> 11) & 1;

    if (flagSet) 
    {
        reg[R_PC] = reg[R_PC] + ExtendSign(instruction & 0b0000011111111111, 11);
    }
    else 
    {
        uint16_t registerIndex = (instruction >> 6) & 0x7;

        reg[R_PC] = reg[registerIndex];
    }

    return;
}

void Register::Ld(const uint16_t & instruction)
{
    // xxxx xxx xxxxxxxxx
    // inst DR  PCOffset9
    uint16_t destinationRegister = (instruction >> 9) & 0x7;
    uint16_t signExtendedOffset = ExtendSign((instruction) & 0b0000000111111111, 9);
    
    reg[destinationRegister] = memory[reg[R_PC] + signExtendedOffset];

    UpdateFlags(static_cast<REGISTER>(destinationRegister));
}

void Register::Ldi(const uint16_t& instruction)
{
    // xxxx xxx xxxxxxxxx
    // inst DR  PCOffset9

    uint16_t destinationRegister = (instruction >> 9) & 0x7;
    uint16_t signExtendedOffset = ExtendSign((instruction) & 0b0000000111111111, 9);

    reg[destinationRegister] = memory[memory[reg[R_PC] + signExtendedOffset]];

    UpdateFlags(static_cast<REGISTER>(destinationRegister));
}

void Register::Ldr(const uint16_t& instruction)
{
    // xxxx xxx xxx xxxxxx
    // inst DR  Reg  Off6

    uint16_t destinationRegister = (instruction >> 9) & 0x7;
    uint16_t baseRegister = (instruction >> 6) & 0x7;

    uint16_t signExtendedOffset = ExtendSign((instruction) & 0b0000000000111111, 6);

    reg[destinationRegister] = memory[reg[baseRegister] + signExtendedOffset];

    UpdateFlags(static_cast<REGISTER>(destinationRegister));
}

void Register::Lea(const uint16_t& instruction) 
{
    // xxxx xxx xxxxxxxxx
    // inst DR  PCOffset9

    uint16_t destinationRegister = (instruction >> 9) & 0x7;
    uint16_t signExtendedOffset = ExtendSign(instruction & 0b0000000111111111, 9);

    reg[destinationRegister] = reg[R_PC] + signExtendedOffset;

    UpdateFlags(static_cast<REGISTER>(destinationRegister));
}

void Register::St(const uint16_t& instruction) 
{
    // xxxx xxx xxxxxxxxx
    // inst SR  PCOffset9

    uint16_t sourceRegister = (instruction >> 9) & 0x7;
    uint16_t signExtendedOffset = ExtendSign((instruction) & 0b0000000111111111, 9);

    memory[reg[R_PC] + signExtendedOffset] = reg[sourceRegister];

    return;
}

void Register::Sti(const uint16_t& instruction)
{
    // xxxx xxx xxxxxxxxx
    // inst SR  PCOffset9

    uint16_t sourceRegister = (instruction >> 9) & 0x7;
    uint16_t signExtendedOffset = ExtendSign((instruction) & 0b0000000111111111, 9);

    memory[memory[reg[R_PC] + signExtendedOffset]] = reg[sourceRegister];

    return;
}

void Register::Str(const uint16_t& instruction)
{
    // xxxx xxx xxx xxxxxx
    // inst SR  BSR Off6

    uint16_t sourceRegister = (instruction >> 9) & 0x7;
    uint16_t baseRegister = (instruction >> 6) & 0x7;
    uint16_t signExtendedOffset = ExtendSign((instruction) & 0b0000000000111111, 6);

    memory[reg[baseRegister] + signExtendedOffset] = reg[sourceRegister];

    return;
}

void Register::Trap(const uint16_t& instruction) 
{
    switch (instruction & 0xFF)
    {
    case TRAP_GETC:
    {
        char letter = std::getchar();
        SetValueInRegister(R_R0, static_cast<uint16_t>(letter) & 0xFF);
        UpdateFlags(R_R0);
        break;
    }
    case TRAP_HALT:
    {
        std::cout << "HALT" << std::endl;
        shouldBeRunning = false;
        break;
    }
    case TRAP_IN:
    {
        std::cout << "Input a character: ";
        char letter = std::getchar();
        SetValueInRegister(R_R0, static_cast<uint16_t>(letter) & 0xFF);
        UpdateFlags(R_R0);
        std::cout << letter;
        break;
    }
    case TRAP_OUT:
    {
        char letter = ReadMemoryAt(GetValueInReg(R_R0)) && 0xFF;
        std::cout << letter;
        break;
    }
    case TRAP_PUTS:
    {
        uint16_t index = 0;
        while (char letter = ReadMemoryAt(GetValueInReg(R_R0) + index) & 0xFF)
        {
            std::cout << letter;
            ++index;
        }

        break;
    }
    case TRAP_PUTSP:
    {
        uint16_t index = 0;
        
        while (char letter = ReadMemoryAt(GetValueInReg(R_R0) + index) & 0xFF << ((index % 2) * 8))
        {
            std::cout << letter;
            ++index;
        }

        break;
    }
    default:
        std::cout << "Error in execution of trap code: " << std::to_string(instruction & 0xFF) << std::endl;
        break;
    }
}

void Register::Br(const uint16_t& instruction)
{
    // xxxx x x x xxxxxxxxx
    // inst n z p PCOffset9
    bool nInstructionFlagSet = (instruction >> 11) & 0x1;
    bool zInstructionFlagSet = (instruction >> 10) & 0x1;
    bool pInstructionFlagSet = (instruction >> 9) & 0x1;

    bool nRegisterSet = reg[R_COND] & 0x1;
    bool zRegisterSet = reg[R_COND] & 0x2;
    bool pRegisterSet = reg[R_COND] & 0x4;

    if ((nInstructionFlagSet && nRegisterSet) || (zInstructionFlagSet && zRegisterSet) || (pInstructionFlagSet && pRegisterSet)) 
    {
        // May have to increment the program counter again or differently according to specification
        reg[R_PC] = reg[R_PC] + ExtendSign(instruction & 0b0000000111111111, 9);
    }
}

void Register::SetValueInRegister(REGISTER regIndex, uint16_t value)
{
    Register::reg[regIndex] = value;
}

void Register::HandleBadOpCode(const uint16_t& instruction) 
{
    //Do something!
}

void Register::ProcessWord()
{
    uint16_t instr = ReadMemoryAt(reg[R_PC]++);
    uint16_t op = instr >> 12;

    switch (op)
    {
    case OP_ADD:
        Register::Add(instr);
        break;
    case OP_AND:
        Register::And(instr);
        break;
    case OP_NOT:
        Register::Not(instr);
        break;
    case OP_BR:
        Register::Br(instr);
        break;
    case OP_JMP:
        Register::Jmp(instr);
        break;
    case OP_JSR:
        Register::Jsr(instr);
        break;
    case OP_LD:
        Register::Ld(instr);
        break;
    case OP_LDI:
        Register::Ldi(instr);
        break;
    case OP_LDR:
        Register::Ldr(instr);
        break;
    case OP_LEA:
        Register::Lea(instr);
        break;
    case OP_ST:
        Register::St(instr);
        break;
    case OP_STI:
        Register::Sti(instr);
        break;
    case OP_STR:
        Register::Str(instr);
        break;
    case OP_TRAP:
        Register::Trap(instr);
        break;
    case OP_RES:
        Register::HandleBadOpCode(instr);
        break;
    case OP_RTI:
        Register::HandleBadOpCode(instr);
        break;
    default:
        Register::HandleBadOpCode(instr);
        break;
    }
}
