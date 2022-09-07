#include "Register.h"

Register::Register()
{
}

Register::~Register()
{
}

uint16_t Register::reg[R_COUNT];
uint16_t Register::memory[MEM_MAX];


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

uint16_t mem_read(uint16_t address) { return 0; }

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
    uint16_t instr = mem_read(reg[R_PC]++);
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
        break;
    case OP_JSR:
        break;
    case OP_LD:
        break;
    case OP_LDI:
        break;
    case OP_LDR:
        break;
    case OP_LEA:
        break;
    case OP_ST:
        break;
    case OP_STI:
        break;
    case OP_STR:
        break;
    case OP_TRAP:
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
