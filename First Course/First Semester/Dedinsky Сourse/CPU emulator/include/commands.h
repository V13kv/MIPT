#ifndef COMMANDS_H
#define COMMANDS_H

#define DEBUG_LEVEL 1
#include "../lib/debug/debug.h"

#define MAX_ENCODED_COMMAND_LENGTH  20
#define MAX_MNEMONICS_STR_LENGTH    20
#define MAX_ARGUMENTS_STR_LENGTH    20

// TODO: void * (for double, int etc argument support)
struct command_t
{
    char    mnemonics[MAX_MNEMONICS_STR_LENGTH]         = {0};
    int     opcode                                      = -1;
    int     MRI                                         = 0;  // Memory, Register, Immediate
    int     arguments_count                             = -1;
    char    arguments[MAX_ARGUMENTS_STR_LENGTH]         = {0};
    int     bytes                                       = 0;
};

enum class COMMAND_OPCODES
{
    HALT,
    PUSH,
    POP,
    ADD,
    SUB,
    MUL,
    DIV,
    OUT,
    IN,
    TOTAL_COMMANDS
};

enum class COMMAND_ARG_COUNT
{
    ZERO,
    ONE,
    TOTAL_ARGS
};  

const command_t commands_table[(int) COMMAND_OPCODES::TOTAL_COMMANDS] = {
    {"halt", (int) COMMAND_OPCODES::HALT, (int) COMMAND_ARG_COUNT::ZERO},
    {"push", (int) COMMAND_OPCODES::PUSH, (int) COMMAND_ARG_COUNT::ONE},
    {"pop", (int) COMMAND_OPCODES::POP, (int) COMMAND_ARG_COUNT::ZERO},
    {"add", (int) COMMAND_OPCODES::ADD, (int) COMMAND_ARG_COUNT::ZERO},
    {"sub", (int) COMMAND_OPCODES::SUB, (int) COMMAND_ARG_COUNT::ZERO},
    {"mul", (int) COMMAND_OPCODES::MUL, (int) COMMAND_ARG_COUNT::ZERO},
    {"div", (int) COMMAND_OPCODES::DIV, (int) COMMAND_ARG_COUNT::ZERO},
    {"out", (int) COMMAND_OPCODES::OUT, (int) COMMAND_ARG_COUNT::ZERO},
    {"in", (int) COMMAND_OPCODES::IN, (int) COMMAND_ARG_COUNT::ZERO},
};


EXIT_CODES getMnemonicsOpcode(char *mnemonics, int *opcode);
EXIT_CODES getCommandMRI(const command_t *const command, int *mri);
EXIT_CODES getCommandArgImmValue(const command_t *const command, double *imm);
EXIT_CODES getCommandArgRegisterOpcode(const command_t *const command, int *regOpcode);

EXIT_CODES resetCommand(command_t *command);


#endif  // COMMANDS_H
