#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#define DEBUG_LEVEL 1
#include "../../lib/debug/debug.h"
#include "../../lib/text/include/text.h"
#include "settings.h"  // FIXME: multiple definition of `MNEMONICS_TABLE'

#define SET_MRI_MEMORY(commandMRI)      commandMRI |= 0b100
#define SET_MRI_REGISTER(commandMRI)    commandMRI |= 0b010
#define SET_MRI_IMMEDIATE(commandMRI)   commandMRI |= 0b001                                                   

enum class ASM_EXIT_CODES
{
    BAD_COMMAND_FORMAT,
    BAD_COMMAND_MEMORY_BRACKETS_USE,
    BAD_COMMAND_ARGUMENTS,
    BAD_MATH_OPERATION_IN_COMMAND_ARG,
    UNKNOWN_MNEMONICS,
    UNKNOWN_COMMAND_REGISTER,
};

struct command_t
{
    char mnemonics[MAX_MNEMONICS_STR_LENGTH]                            = {};
    int opcode                                                          = 0;

    char arguments[MAX_ARGUMENTS_PER_COMMAND][MAX_ARGUMENT_STR_LENGTH]  = {};
    int argsMRI[MAX_ARGUMENTS_PER_COMMAND]                              = {}; // MRI <-> Memory, Register, Immediate arg
    
    int argumentsCount                                                  = 0;
    int MRI                                                             = 0; // MRI <-> Memory, Register, Immediate (general)
};

EXIT_CODES assembly(text_t *code, char *outputFile);
EXIT_CODES parseCommand(text_line_t *line, command_t *command);
EXIT_CODES checkMnemonics(char *mnemonics);
EXIT_CODES setCommandMnemonics(command_t *command, char *mnemonics);
EXIT_CODES setCommandOpcode(command_t *command);
EXIT_CODES parseCommandArguments(command_t *command, text_line_t *line, const int argsStart, const int argsEnd);
EXIT_CODES parseArgument(command_t *command, int *argNumber, text_line_t *line, const int argsStart, int *argEnd);
EXIT_CODES checkRegisterForCorrectness(char *reg);

#endif  // ASSEMBLER_H
