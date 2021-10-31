#ifndef REGISTERS_H
#define REGISTERS_H

enum class REGISTER_EXIT_CODES
{
    NO_SUCH_REGISTER_FOUND,
};

struct register_t
{
    char *name  = NULL;
    int opcode  = -1;
    int value   = -1;
};

enum class REGISTER_OPCODES
{
    INVALID_OPCODE = -1,
    AX,
    BX,
    CX,
    DX,
    IP,
    TOTAL_REGISTERS
};

// TODO: rename to REGISTERS_TABLE
const register_t registers[(int) REGISTER_OPCODES::TOTAL_REGISTERS] = {
    {"ax", (int) REGISTER_OPCODES::AX},
    {"bx", (int) REGISTER_OPCODES::BX},
    {"cx", (int) REGISTER_OPCODES::CX},
    {"dx", (int) REGISTER_OPCODES::DX},
    {"ip", (int) REGISTER_OPCODES::IP},
};


bool registerNameIsCorrect(char reg[]);


#endif  // REGISTERS_H
