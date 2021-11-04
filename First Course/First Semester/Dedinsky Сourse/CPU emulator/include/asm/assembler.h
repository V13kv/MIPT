#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <stdbool.h>
#include <stddef.h>  // for size_t

#define DEBUG_LEVEL 1
#include "../../lib/debug/debug.h"
#include "../../lib/text/include/text.h"

#include "settings.h"                                               
#include "labels.h"

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
    // TODO: char to byte
    byte byteData[MAX_ENCODED_COMMAND_LENGTH]   = {};
    int bytes                                   = 0;
};

// ******TODO: add specialCommand field (determines whether instruction is one of jmp, call *etc...)
struct command_t
{
    char mnemonics[MAX_MNEMONICS_STR_LENGTH]                            = {};
    int opcode                                                          = 0;

    char arguments[MAX_ARGUMENTS_PER_COMMAND][MAX_ARGUMENT_STR_LENGTH]  = {};
    int argsMRI[MAX_ARGUMENTS_PER_COMMAND]                              = {}; // MRI <-> Memory, Register, Immediate (arg)
    
    size_t argumentsCount                                               = 0;
    int MRI                                                             = 0; // MRI <-> Memory, Register, Immediate (general)
    bool isSpecialCommand                                               = false; // special instrs are jmp, call (1), otherwise 0

    encoded_command_t encoded                                           = {};
};


#define ENCODE_COMMAND_ARGS_COUNT(commandArgsCount) commandArgsCount << 5
#define ENCODE_COMMAND_MRI(commandMRI)              commandMRI << 2
#define ARG_IS_REGISTER(argMRI)                     !!(argMRI & 0b010)

#define SET_MRI_MEMORY(commandMRI)                  commandMRI |= 0b100
#define SET_MRI_REGISTER(commandMRI)                commandMRI |= 0b010
#define SET_MRI_IMMEDIATE(commandMRI)               commandMRI |= 0b001 

EXIT_CODES assembly(text_t *code, char *outputFile);

#endif  // ASSEMBLER_H
