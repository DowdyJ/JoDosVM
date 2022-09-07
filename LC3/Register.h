#pragma once
#include <cstdint>
#define MEM_MAX (1 << 15)
#define private public

class Register
{

public:
    const enum OPCODE
    {
        OP_BR = 0, /* branch */
        OP_ADD,    /* add  */
        OP_LD,     /* load */
        OP_ST,     /* store */
        OP_JSR,    /* jump register */
        OP_AND,    /* bitwise and */
        OP_LDR,    /* load register */
        OP_STR,    /* store register */
        OP_RTI,    /* unused */
        OP_NOT,    /* bitwise not */
        OP_LDI,    /* load indirect */
        OP_STI,    /* store indirect */
        OP_JMP,    /* jump */
        OP_RES,    /* reserved (unused) */
        OP_LEA,    /* load effective address */
        OP_TRAP    /* execute trap */
    };

    const enum REGISTER
    {
        R_R0 = 0,
        R_R1,
        R_R2,
        R_R3,
        R_R4,
        R_R5,
        R_R6,
        R_R7,
        R_PC,
        R_COND,
        R_COUNT
    };

    const enum FLAGS
    {
        FL_POS = 1 << 0, /* P */
        FL_ZRO = 1 << 1, /* Z */
        FL_NEG = 1 << 2, /* N */
    };

	Register();
	
    ~Register();

    static void UpdateFlags(REGISTER regIndex);

    static void ProcessProgram();

    #pragma region OPCODE DEFINITIONS

    static void Add(const uint16_t& instruction);

    static void And(const uint16_t& instruction);

    static void Not(const uint16_t& instruction);

    static void Jmp(const uint16_t& instruction);

    static void Jsr(const uint16_t& instruction);

    static void Br(const uint16_t& instruction);

    static void Ld(const uint16_t& instruction);

    static void Ldi(const uint16_t& instruction);
    
    static void Ldr(const uint16_t& instruction);

    static void Lea(const uint16_t& instruction);
    
    static void St(const uint16_t& instruction);

    static void Sti(const uint16_t& instruction);

    static void Str(const uint16_t& instruction);

#pragma endregion

    static void HandleBadOpCode(const uint16_t& instruction);



    inline static uint16_t GetValueInReg(REGISTER regIndex) 
    {
        return reg[regIndex];
    }

private:
    static void SetValueInRegister(REGISTER regIndex, uint16_t value);


    static inline uint16_t ExtendSign(const uint16_t& value, const int& bitCount)
    {
        uint16_t valueCopy = value;
        if ((valueCopy >> (bitCount - 1)) & 1) {
            valueCopy |= (0xFFFF << bitCount);
        }
        return valueCopy;
    }

    static void ProcessWord();

    static uint16_t reg[R_COUNT];

    static uint16_t memory[MEM_MAX];

};



#undef private