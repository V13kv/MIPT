#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include <stdio.h>

#define DEBUG_LEVEL 1
#include "../../lib/debug/debug.h"  // for EXIT_CODES

#include "../commands.h"
#include "../../lib/text/include/text.h"


EXIT_CODES translateCode(text_t *text, const char *const output_file_name);

EXIT_CODES parseLine(text_line_t *line, command_t *command);
EXIT_CODES getMnemonicsOpcode(char *mnemonics, unsigned int *opcode);
EXIT_CODES exportEncodedCommand(command_t *command, FILE *fs);
EXIT_CODES encodeCommand(command_t *command, char *encodedCommand);


#endif  // ASSEMBLER_H
