#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <stdio.h>

#define DEBUG_LEVEL 1
#include "../../lib/debug/debug.h"

#include "../../lib/text/include/text.h"
#include "../commands.h"

#define ENCODE_COMMAND_OPCODE(commandOpcode)        commandOpcode << 4
#define ENCODE_COMMAND_ARGS_COUNT(commandArgsCount) commandArgsCount << 2
#define ENCODE_COMMAND_MRI(commandMRI)              commandMRI << 5

EXIT_CODES assembly(const text_t *const code, const char *const output_file_name);
EXIT_CODES parseLine(const text_line_t *const line, command_t *const command);
EXIT_CODES encodeCommand(command_t *const command, char *const encodedCommand);

EXIT_CODES exportEncodedCommand(const char *const encodedCommand, const int bytesToExport, FILE *fs);

#endif  // ASSEMBLER_H
