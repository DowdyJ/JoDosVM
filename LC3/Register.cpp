#include "Register.h"

Register::Register()
{
}

Register::~Register()
{
}

uint16_t Register::reg[R_COUNT];


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

    if ((instruction >> 5) & 1) // alt-add mode
    {
        uint16_t valueToAdd = (instruction) & 0x1f;
        valueToAdd = ExtendSign(valueToAdd, 5);

        uint16_t firstRegister = (instruction >> 6) & 0x7;
        uint16_t destinationRegister = (instruction >> 12) & 0x7;

        reg[destinationRegister] = reg[firstRegister] + valueToAdd;
    }
    else // first add mode 
    {
        uint16_t firstRegister = (instruction >> 6) & 0x7;
        uint16_t secondRegister = (instruction) & 0x7;
        uint16_t destinationRegister = (instruction >> 12) & 0x7;

        reg[destinationRegister] = reg[firstRegister] + reg[secondRegister];

        UpdateFlags(static_cast<REGISTER>(destinationRegister));
    }

    return;
}

void Register::ProcessWord()
{
    uint16_t instr = mem_read(reg[R_PC]++);
    uint16_t op = instr >> 12;

    switch (op)
    {
    case OP_ADD:
        break;
    case OP_AND:
        break;
    case OP_NOT:
        break;
    case OP_BR:
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
    case OP_RTI:
    default:
        // Bad opcode
        break;
    }
}
