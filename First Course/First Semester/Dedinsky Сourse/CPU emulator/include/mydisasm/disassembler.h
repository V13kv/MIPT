#ifndef DISASSEMBLER_H
#define DISASSEMBLER_H


#include <stdio.h>

#define DEBUG_LEVEL 1
#include "../../lib/debug/debug.h"  // for EXIT_CODES

#include "../commands.h"
#include "../../lib/text/include/text.h"

#define GET_MNEMONICS(opcode) commands_table[opcode].mnemonics
#define GET_ENCODED_COMMAND_OPCODE(encodedCommand) (encodedCommand & 0xF0) >> 4
#define GET_ENCODED_COMMAND_ARGS_COUNT(encodedCommand) (encodedCommand & 0xC) >> 2

//TODO: own command_t structure for disassembly
//TODO: move enum class to general.h header file
enum class ASM_EXIT_CODE
{
    BAD_COMMAND_STRUCTURE_FIELDS,
    NO_SUCH_MNEMONICS_FOUND,
};

EXIT_CODES disassembly(text_t *code, const char *const output_file_name);
EXIT_CODES decodeCommand(char encodedCommand, command_t *command);
EXIT_CODES decodeCommandArg(char encodedArgument, char *decodedArgument);

EXIT_CODES exportDecodedCommand(command_t *command, FILE *fs);



#endif  // DISASSEMBLER_H
