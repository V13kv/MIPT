#include <string.h>  // for strcmp
#include <stdlib.h>  // for calloc && free
#include <stdio.h>  // for sscanf

#include "../../include/myasm/assembler.h"
#include "../../include/commands.h"

//TODO: my asm code syntax checker

enum class ASM_EXIT_CODE
{
    BAD_COMMAND_STRUCTURE_FIELDS,
    NO_SUCH_MNEMONICS_FOUND,
};

EXIT_CODES encodeCommand(command_t *command, char *encodedCommand)
{
    // Error check
    if (command == NULL || encodedCommand == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::PASSED_OBJECT_IS_NULLPTR);
        return EXIT_CODES::PASSED_OBJECT_IS_NULLPTR;
    }

    // TODO: basicCommandCheck function
    if (command->mnemonics == NULL || command->arguments == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(ASM_EXIT_CODE::BAD_COMMAND_STRUCTURE_FIELDS);
        return EXIT_CODES::BAD_OBJECT_PASSED;
    }

    // Encoding
    unsigned int encoded = command->opcode << 4;
    encoded = encoded | (command->arguments_count << 2);
    encodedCommand[0] = (char) encoded;

    for (int arg = 0; arg < command->arguments_count; ++arg)
    {
        int imm = atoi(command->arguments);
        encodedCommand[arg + 1] = (char) imm;
    }

    return EXIT_CODES::NO_ERRORS;
}

EXIT_CODES exportEncodedCommand(command_t *command, FILE *fs)
{
    // Error check
    if (command == NULL || fs == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::PASSED_OBJECT_IS_NULLPTR);
        return EXIT_CODES::PASSED_OBJECT_IS_NULLPTR;
    }

    if (command->mnemonics == NULL || command->arguments == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(ASM_EXIT_CODE::BAD_COMMAND_STRUCTURE_FIELDS);
        return EXIT_CODES::BAD_OBJECT_PASSED;
    }

    // Export
    char *encodedCommand = (char *) calloc(command->arguments_count + 1, sizeof(char));
    IS_OK_W_EXIT(encodeCommand(command, encodedCommand));
    
    fwrite(encodedCommand, sizeof(encodedCommand[0]), command->arguments_count + 1, fs);

    free(encodedCommand);
    return EXIT_CODES::NO_ERRORS;
}

EXIT_CODES getMnemonicsOpcode(char *mnemonics, unsigned int *opcode)
{
    // Error check
    if (mnemonics == NULL || opcode == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::BAD_OBJECT_PASSED);
        return EXIT_CODES::BAD_OBJECT_PASSED;
    }

    // Get command opcode
    for (int command = 0; command < (int) OPCODES::TOTAL_COMMANDS; ++command)
    {
        if (!strcmp(mnemonics, commands_table[command].mnemonics))
        {
            *opcode = commands_table[command].opcode;
            return EXIT_CODES::NO_ERRORS;
        }
    }

    PRINT_ERROR_TRACING_MESSAGE(ASM_EXIT_CODE::NO_SUCH_MNEMONICS_FOUND);
    return EXIT_CODES::BAD_OBJECT_PASSED;
}

// TODO: support not only of imm values, but mem, reg, imm and combinations of them
EXIT_CODES parseLine(text_line_t *line, command_t *command)
{
    // Error check
    if (line == NULL || command == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::BAD_OBJECT_PASSED);
        return EXIT_CODES::BAD_OBJECT_PASSED;
    }
    
    // Fill `command` structure with contents of the current `line`
    char mnemonics[10] = {0};
    char arguments[10] = {0};
    int ret = sscanf(line->beginning, "%s %s", mnemonics, arguments);
    if (ret == 0 || ret == EOF)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::BAD_STD_FUNC_RESULT);
        return EXIT_CODES::BAD_STD_FUNC_RESULT;
    }

    unsigned int opcode = -1;
    IS_OK_WO_EXIT(getMnemonicsOpcode(mnemonics, &opcode));

    command->mnemonics = mnemonics;
    command->arguments_count = (int) strlen(arguments);
    command->arguments = arguments;
    command->opcode = opcode;

    return EXIT_CODES::NO_ERRORS;
}

EXIT_CODES translateCode(text_t *text, const char *const output_file_name)
{
    // Error check
    if (text == NULL || text->lines == NULL || output_file_name == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::BAD_OBJECT_PASSED);
        return EXIT_CODES::BAD_OBJECT_PASSED;
    }
    for (int line = 0; line < text->lines_count; ++line)
    {
        if (text->lines[line].beginning == NULL)
        {
            PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::BAD_OBJECT_PASSED);
            return EXIT_CODES::BAD_OBJECT_PASSED;
        }
    }

    // Parse each source code line
    FILE *fs = fopen(output_file_name, "wb");
    if (fs == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::BAD_STD_FUNC_RESULT);
        return EXIT_CODES::BAD_STD_FUNC_RESULT;
    }

    command_t command = {};
    for (int line = 0; line < text->lines_count; ++line)
    {
        IS_OK_W_EXIT(parseLine(&text->lines[line], &command));
        
        IS_OK_W_EXIT(exportEncodedCommand(&command, fs));
    }

    fclose(fs);

    return EXIT_CODES::NO_ERRORS;
}
