#ifndef COMMANDS_H
#define COMMANDS_H

#define DEBUG_LEVEL 1
#include "../lib/debug/debug.h"

#include "constants.h"

enum class COMMAND_EXIT_CODES
{
    WRONG_COMMAND_FORMAT,

};

struct command_t
{
    char    mnemonics[MAX_MNEMONICS_STR_LENGTH]                             = {0};
    int     opcode                                                          = -1;
    int     MRI                                                             = 0;  // Memory, Register, Immediate
    int     arguments_count                                                 = -1;
    char    arguments[MAX_ARGUMENTS_PER_COMMAND][MAX_ARGUMENT_STR_LENGTH]   = {};
    int     bytes                                                           = 0;
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
    JMP,
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
    {"jmp", (int) COMMAND_OPCODES::JMP, (int) COMMAND_ARG_COUNT::ONE},
};

// #define GET_ARGS_COUNT_VIA_MRI(commandMRI)          (commandMRI & 0b001) + ((commandMRI & 0b010) >> 1)
#define SET_MRI_MEMORY(commandMRI)                  commandMRI |= 0b100
#define SET_MRI_REGISTER(commandMRI)                commandMRI |= 0b010
#define SET_MRI_IMMEDIATE(commandMRI)               commandMRI |= 0b001
#define MRI_HAS_MEMORY(commandMRI)                  !!(commandMRI & 0b100)
#define MRI_HAS_REGISTER(commandMRI)                !!(commandMRI & 0b010)
#define MRI_HAS_IMMEDIATE(commandMRI)               !!(commandMRI & 0b001)
#define GET_MNEMONICS(opcode)                       commands_table[opcode].mnemonics

EXIT_CODES getMnemonicsOpcode(const char *const mnemonics, int *const opcode);
EXIT_CODES getCommandMRI(const command_t *const command, int *const mri);
EXIT_CODES getCommandArgImmValue(const command_t *const command, double *const imm);
EXIT_CODES getCommandArgRegisterOpcode(const command_t *const command, int *const regOpcode);

EXIT_CODES resetCommand(command_t *command);


#endif  // COMMANDS_H
