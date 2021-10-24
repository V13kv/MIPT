#include <stdlib.h>  // for calloc
#include <string.h>  // for strlen

#include "../../include/mydisasm/disassembler.h"

//TODO: support of mem, reg, imm arguments and combinations of them
EXIT_CODES decodeCommandArg(char encodedArgument, char *decodedArgument)
{
    // Error check
    if (decodedArgument == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::PASSED_OBJECT_IS_NULLPTR);
        return EXIT_CODES::PASSED_OBJECT_IS_NULLPTR;
    }
    
    // printf("encodedArgument: %d\n", encodedArgument);
    *decodedArgument = (int) encodedArgument + 0x30;

    return EXIT_CODES::NO_ERRORS;
}

// TODO: support of mem, reg, imm via command->arguments = (char **) calloc(...)
EXIT_CODES decodeCommand(char *encodedData, int baseByte, command_t *command)
{
    // Error check
    if (encodedData == NULL || command == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::PASSED_OBJECT_IS_NULLPTR);
        return EXIT_CODES::PASSED_OBJECT_IS_NULLPTR;
    }

    // Decoding instruction header
    command->opcode = GET_ENCODED_COMMAND_OPCODE(encodedData[baseByte]);
    command->arguments_count = GET_ENCODED_COMMAND_ARGS_COUNT(encodedData[baseByte]);
    command->mnemonics = GET_MNEMONICS(command->opcode);

    // Decoding instruction arguments
    command->arguments = (char *) calloc(command->arguments_count, sizeof(char));  // TODO: error check
    for (int arg = 0; arg < command->arguments_count; ++arg)
    {
        IS_OK_WO_EXIT(decodeCommandArg(encodedData[baseByte + arg + 1], &command->arguments[arg]));
    }

    return EXIT_CODES::NO_ERRORS;
}

EXIT_CODES exportDecodedCommand(command_t *command, FILE *fs)
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
    fputs(command->mnemonics, fs);
    fputs(" ", fs);

    for (int arg = 0; arg < command->arguments_count; ++arg)
    {
        fputs(&command->arguments[arg], fs);
        fputs(" ", fs);
    }
    fputs("\n", fs);

    return EXIT_CODES::NO_ERRORS;
}


EXIT_CODES disassembly(text_t *byteCode, const char *const output_file_name)
{
    // Error check
    if (byteCode == NULL || byteCode->data == NULL || output_file_name == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::PASSED_OBJECT_IS_NULLPTR);
        return EXIT_CODES::PASSED_OBJECT_IS_NULLPTR;
    }

    // Disassembly
    FILE *fs = fopen(output_file_name, "w");
    if (fs == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::BAD_STD_FUNC_RESULT);
        return EXIT_CODES::BAD_STD_FUNC_RESULT;
    }

    command_t command = {};
    for (int byte = 0; byte < byteCode->size; byte += command.arguments_count + 1)
    {
        IS_OK_W_EXIT(decodeCommand(byteCode->data, byte, &command));
        IS_OK_W_EXIT(exportDecodedCommand(&command, fs));

        free(command.arguments);
    }
}
