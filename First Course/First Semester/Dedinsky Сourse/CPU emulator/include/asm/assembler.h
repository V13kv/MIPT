#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <stdbool.h>

#define DEBUG_LEVEL 1
#include "../../lib/debug/debug.h"
#include "../../lib/text/include/text.h"
#include "labels.h"
#include "settings.h"  // FIXME: multiple definition of `MNEMONICS_TABLE'                                                 

extern int globalOffset;

typedef unsigned char byte;

enum class ASM_EXIT_CODES
{
    BAD_COMMAND_FORMAT,
    BAD_COMMAND_MEMORY_BRACKETS_USE,
    BAD_COMMAND_ARGUMENTS,
    BAD_MATH_OPERATION_IN_COMMAND_ARG,
    UNKNOWN_MNEMONICS,
    UNKNOWN_COMMAND_REGISTER,
    BAD_LABEL_NAME,
    BAD_LABEL_FORMAT,
};

struct encoded_command_t
{
    char byteData[MAX_ENCODED_COMMAND_LENGTH]   = {};
    int bytes                                   = 0;
};

// TODO: add specialCommand field (determines whether instruction is one of jmp, call *etc...)
struct command_t
{
    char mnemonics[MAX_MNEMONICS_STR_LENGTH]                            = {};
    int opcode                                                          = 0;

    char arguments[MAX_ARGUMENTS_PER_COMMAND][MAX_ARGUMENT_STR_LENGTH]  = {};
    int argsMRI[MAX_ARGUMENTS_PER_COMMAND]                              = {}; // MRI <-> Memory, Register, Immediate (arg)
    
    size_t argumentsCount                                               = 0;
    int MRI                                                             = 0; // MRI <-> Memory, Register, Immediate (general)
    bool isSpecialCommand                                               = 0; // special instrs are jmp, call (1), otherwise 0

    encoded_command_t encoded                                           = {};
};

#define ENCODE_COMMAND_ARGS_COUNT(commandArgsCount) commandArgsCount << 5
#define ENCODE_COMMAND_MRI(commandMRI)              commandMRI << 2
#define ARG_IS_REGISTER(argMRI)                     !!(argMRI & 0b010)

#define SET_MRI_MEMORY(commandMRI)                  commandMRI |= 0b100
#define SET_MRI_REGISTER(commandMRI)                commandMRI |= 0b010
#define SET_MRI_IMMEDIATE(commandMRI)               commandMRI |= 0b001 


EXIT_CODES assembly(text_t *code, char *outputFile);

EXIT_CODES parseCommand(text_line_t *line, command_t *command, labels_t *unprocCommandArgLabels);
EXIT_CODES isSpecialInstruction(command_t *command, bool *isSpecInstr);
EXIT_CODES hasArguments(char *mnemonics, bool *hasArgs);
EXIT_CODES checkMnemonics(char *mnemonics);
EXIT_CODES setCommandMnemonics(command_t *command, char *mnemonics);
EXIT_CODES setCommandOpcode(command_t *command);
EXIT_CODES parseCommandArguments(command_t *command, text_line_t *line, int argsStart, int argsEnd, labels_t *unprocCommandArgLabels);
EXIT_CODES parseArgument(command_t *command, int *argNumber, text_line_t *line, int *argStart);
EXIT_CODES checkRegisterForCorrectness(char *reg);
EXIT_CODES getArgumentsMathOperation(text_line_t *line, int *argStart, char *mathOP);


EXIT_CODES encodeCommand(command_t *command);
EXIT_CODES encodeRegisterArgument(command_t *command, char *regStr);
EXIT_CODES encodeImmediateArgument(command_t *command, char *immStr);
EXIT_CODES exportEncodedCommand(command_t *command, FILE *fs);

EXIT_CODES fillUnprocCommandArgLabels(labels_t *unproc, labels_t *labels, FILE *fs);

EXIT_CODES resetCommand(command_t *command);


#endif  // ASSEMBLER_H
