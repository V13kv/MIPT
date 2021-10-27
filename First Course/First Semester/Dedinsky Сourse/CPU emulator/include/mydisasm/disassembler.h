#ifndef DISASSEMBLER_H
#define DISASSEMBLER_H

#include <stdio.h>

#define DEBUG_LEVEL 1
#include "../../lib/debug/debug.h"  // for EXIT_CODES

#include "../commands.h"
#include "../../lib/text/include/text.h"

// TODO: move general macroses to separate file
#define MRI_HAS_IMMEDIATE(commandMRI)               !!(commandMRI & 0b001)
#define MRI_HAS_REGISTER(commandMRI)                !!(commandMRI & 0b010)
#define MRI_HAS_MEMORY(commandMRI)                  !!(commandMRI & 0b100)


#define GET_MNEMONICS(opcode)                           commands_table[opcode].mnemonics
#define GET_ENCODED_COMMAND_OPCODE(encodedByte)         (encodedByte & 0xF0) >> 4
#define GET_ENCODED_COMMAND_MRI(encodedByte)            (encodedByte & 0b11100000) >> 5
#define GET_ENCODED_COMMAND_ARGS_COUNT(encodedByte)     (encodedByte & 0b1100) >> 2
#define GET_ENCODED_COMMAND_REGISTER_NAME(encodedByte)  registers[encodedByte].name
#define GET_ENCODED_COMMAND_DOUBLE_IMM_VALUE(byteCodeP) *( ((double *) byteCodeP) )


EXIT_CODES disassembly(text_t *code, const char *const output_file_name);
EXIT_CODES decodeCommand(char *byteCode, command_t *command);
EXIT_CODES decodeCommandArgs(char *byteCode, command_t *command);

EXIT_CODES exportDecodedCommand(command_t *command, FILE *fs);



#endif  // DISASSEMBLER_H
