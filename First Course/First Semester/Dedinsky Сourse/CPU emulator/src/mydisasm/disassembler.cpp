#include <stdlib.h>  // for calloc
#include <string.h>  // for strlen && strcpy

#include "../../include/mydisasm/disassembler.h"
#include "../../include/commands.h"

EXIT_CODES decodeCommandArgs(char *byteCode, command_t *const command)
{
    // Error check
    if (byteCode == NULL || command == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::PASSED_OBJECT_IS_NULLPTR);
        return EXIT_CODES::PASSED_OBJECT_IS_NULLPTR;
    }
    
    // Decoding instruction arguments
    if (MRI_HAS_MEMORY(command->MRI))
    {
        strcat(command->arguments, "[");
    }

    if (MRI_HAS_REGISTER(command->MRI))
    {
        strcat(command->arguments, GET_ENCODED_COMMAND_REGISTER_NAME(byteCode[command->bytes]));
        command->bytes++;
    }

    if (MRI_HAS_IMMEDIATE(command->MRI))
    {
        if (MRI_HAS_REGISTER(command->MRI))
        {
            strcat(command->arguments, " + ");
        }

        // TODO: normal conversion from byteCode -> double -> char *
        double imm = GET_ENCODED_COMMAND_DOUBLE_IMM_VALUE(&byteCode[command->bytes]);
        char cImm[5] = {0};
        snprintf(cImm, 5, "%lf", imm);
        
        strcat(command->arguments, cImm);
        command->bytes += (int) sizeof(double);
    }

    if (MRI_HAS_MEMORY(command->MRI))
    {
        strcat(command->arguments, "]");
    }

    return EXIT_CODES::NO_ERRORS;
}

EXIT_CODES decodeCommand(char *byteCode, command_t *const command)
{
    // Error check
    if (byteCode == NULL || command == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::PASSED_OBJECT_IS_NULLPTR);
        return EXIT_CODES::PASSED_OBJECT_IS_NULLPTR;
    }

    // Decoding 1st instr header byte (opcode && arguments_count)
    command->opcode             = GET_ENCODED_COMMAND_OPCODE(byteCode[command->bytes]);
    command->arguments_count    = GET_ENCODED_COMMAND_ARGS_COUNT(byteCode[command->bytes]);
    strcpy(command->mnemonics, GET_MNEMONICS(command->opcode));
    command->bytes++;

    // Decoding 2nd instr header byte (MRI (mem, reg, imm))
    command->MRI                = GET_ENCODED_COMMAND_MRI(byteCode[command->bytes]);
    command->bytes++;

    IS_OK_W_EXIT(decodeCommandArgs(byteCode, command));

    // printf("command->mnemonics: %s\n", command->mnemonics);
    // printf("command->opcode: %d\n", command->opcode);
    // printf("command->MRI: %u\n", command->MRI);
    // printf("command->arguments_count: %d\n", command->arguments_count);
    // printf("command->bytes: %d\n", command->bytes);
    // printf("command->arguments: %s\n\n", command->arguments);

    return EXIT_CODES::NO_ERRORS;
}

EXIT_CODES exportDecodedCommand(const command_t *const command, FILE *fs)
{
    // Error check
    if (command == NULL || fs == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::PASSED_OBJECT_IS_NULLPTR);
        return EXIT_CODES::PASSED_OBJECT_IS_NULLPTR;
    }

    // Export
    fputs(command->mnemonics, fs);
    fputs(" ", fs);
    fputs(command->arguments, fs);
    fputs("\n", fs);

    return EXIT_CODES::NO_ERRORS;
}

EXIT_CODES disassembly(const text_t *const byteCode, const char *const output_file_name)
{
    // Error check
    if (byteCode == NULL || byteCode->data == NULL || output_file_name == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::PASSED_OBJECT_IS_NULLPTR);
        return EXIT_CODES::PASSED_OBJECT_IS_NULLPTR;
    }

    // Output listing file
    FILE *fs = fopen(output_file_name, "w");
    if (fs == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::BAD_STD_FUNC_RESULT);
        return EXIT_CODES::BAD_STD_FUNC_RESULT;
    }

    // TODO: сделать размер константного размера либо каллок, чтобы не каллочить в цикле и не фришить в цикле
    // Parse bytecode
    command_t command = {};
    for (size_t byte = 0; byte < byteCode->size; byte += command.bytes)//command.arguments_count + 1)
    {
        IS_OK_W_EXIT(resetCommand(&command));

        IS_OK_W_EXIT(decodeCommand(&byteCode->data[byte], &command));
        IS_OK_W_EXIT(exportDecodedCommand(&command, fs));
    }

    fclose(fs);

    return EXIT_CODES::NO_ERRORS;
}
