#ifndef DISASSEMBLER_H
#define DISASSEMBLER_H

#include <stdio.h>

#define DEBUG_LEVEL 1
#include "../../lib/debug/debug.h"
#include "../../lib/text/include/text.h"

#include "../registers.h"
#include "../commands.h"

#define GET_ENCODED_COMMAND_OPCODE(encodedByte)         (encodedByte & 0xF0) >> 4
#define GET_ENCODED_COMMAND_MRI(encodedByte)            (encodedByte & 0b11100000) >> 5
#define GET_ENCODED_COMMAND_ARGS_COUNT(encodedByte)     (encodedByte & 0b1100) >> 2
#define GET_ENCODED_COMMAND_REGISTER_NAME(encodedByte)  registers[(int)encodedByte].name
#define GET_ENCODED_COMMAND_DOUBLE_IMM_VALUE(byteCodeP) *( (double *) byteCodeP )

EXIT_CODES disassembly(const text_t *const code, const char *const output_file_name);
EXIT_CODES decodeCommand(char *byteCode, command_t *const command);
EXIT_CODES decodeCommandArgs(char *byteCode, command_t *const command);

EXIT_CODES exportDecodedCommand(const command_t *const command, FILE *fs);

#endif  // DISASSEMBLER_H
