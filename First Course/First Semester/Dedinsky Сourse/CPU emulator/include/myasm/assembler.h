#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <stdio.h>

#define DEBUG_LEVEL 1
#include "../../lib/debug/debug.h"  // for EXIT_CODES

#include "../commands.h"
#include "../../lib/text/include/text.h"

#define ENCODE_COMMAND_OPCODE(commandOpcode)        commandOpcode << 4
#define ENCODE_COMMAND_ARGS_COUNT(commandArgsCount) commandArgsCount << 2
#define ENCODE_COMMAND_MRI(commandMRI)              commandMRI << 5

#define GET_ARGS_COUNT_VIA_MRI(commandMRI)          (commandMRI & 0b001) + ((commandMRI & 0b010) >> 1)
#define MRI_HAS_IMMEDIATE(commandMRI)               !!(commandMRI & 0b001)
#define MRI_HAS_REGISTER(commandMRI)                !!(commandMRI & 0b010)
#define MRI_HAS_MEMORY(commandMRI)                  !!(commandMRI & 0b100)


EXIT_CODES translateCode(text_t *text, const char *const output_file_name);
EXIT_CODES parseLine(text_line_t *line, command_t *command);
EXIT_CODES encodeCommand(command_t *const command, char *encodedCommand);

EXIT_CODES exportEncodedCommand(command_t *command, int bytesToExport, FILE *fs);


#endif  // ASSEMBLER_H
