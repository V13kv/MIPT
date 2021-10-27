#ifndef ASM_GENERAL
#define ASM_GENERAL


// enum class ASM_EXIT_CODE
// {
//     BAD_COMMAND_STRUCTURE_FIELDS,
//     NO_SUCH_MNEMONICS_FOUND,
//     NO_SUCH_REGISTER_FOUND,
// };

struct register_t
{
    char *name = NULL;
    int opcode = -1;
};

enum class REGISTER_OPCODES
{
    AX,
    BX,
    CX,
    DX,
    TOTAL_REGISTERS
};

const register_t registers[(int) REGISTER_OPCODES::TOTAL_REGISTERS] = {
    {"ax", (int) REGISTER_OPCODES::AX},
    {"bx", (int) REGISTER_OPCODES::BX},
    {"cx", (int) REGISTER_OPCODES::CX},
    {"dx", (int) REGISTER_OPCODES::DX},
};


#endif  // ASM_GENERAL
