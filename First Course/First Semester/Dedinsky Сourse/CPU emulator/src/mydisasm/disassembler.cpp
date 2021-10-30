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
    unsigned int arg = 0;
    if (MRI_HAS_REGISTER(command->MRI))
    {
        strcpy(command->arguments[arg++], GET_ENCODED_COMMAND_REGISTER_NAME(byteCode[command->bytes++]));
    }

    if (MRI_HAS_IMMEDIATE(command->MRI))
    {     
        // TODO: normal conversion from byteCode -> double -> char *
        double imm = GET_ENCODED_COMMAND_DOUBLE_IMM_VALUE(&byteCode[command->bytes]);
        char cImm[5] = {0};
        snprintf(cImm, 5, "%lf", imm);

        strcpy(command->arguments[arg++], cImm);
        
        command->bytes += (int) sizeof(double);
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
    command->arguments_count    = GET_ENCODED_COMMAND_ARGS_COUNT(byteCode[command->bytes++]);
    strcpy(command->mnemonics, GET_MNEMONICS(command->opcode));

    // Decoding 2nd instr header byte (MRI (mem, reg, imm))
    command->MRI                = GET_ENCODED_COMMAND_MRI(byteCode[command->bytes++]);
    
    // Decode command arguments
    IS_OK_W_EXIT(decodeCommandArgs(byteCode, command));

    // printf("command->mnemonics: %s\n", command->mnemonics);
    // printf("command->opcode: %d\n", command->opcode);
    // printf("command->MRI: %u\n", command->MRI);
    // printf("command->arguments_count: %d\n", command->arguments_count);
    // printf("command->bytes: %d\n", command->bytes);
    // printf("command->arguments: %s\n\n", command->arguments);

    return EXIT_CODES::NO_ERRORS;
}

EXIT_CODES exportBeautifiedArgs(const command_t *const command, FILE *fs)
{
    // Error check
    if (command == NULL || fs == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::PASSED_OBJECT_IS_NULLPTR);
        return EXIT_CODES::PASSED_OBJECT_IS_NULLPTR;
    }

    // Export beautified args
    if (MRI_HAS_MEMORY(command->MRI))
    {
        fputc('[', fs);
    }

    unsigned int arg = 0;
    if (MRI_HAS_REGISTER(command->MRI))
    {
        fputs(command->arguments[arg++], fs);
    }

    if (MRI_HAS_IMMEDIATE(command->MRI))
    {
        if (MRI_HAS_REGISTER(command->MRI))
        {
            fputs(" + ", fs);
        }

        fputs(command->arguments[arg++], fs);
    }

    if (MRI_HAS_MEMORY(command->MRI))
    {
        fputc(']', fs);
    }

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
    IS_OK_W_EXIT(exportBeautifiedArgs(command, fs));
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

    // Parse bytecode
    command_t command = {};
    for (size_t byte = 0; byte < byteCode->size; byte += command.bytes)
    {
        IS_OK_W_EXIT(resetCommand(&command));

        IS_OK_W_EXIT(decodeCommand(&byteCode->data[byte], &command));
        IS_OK_W_EXIT(exportDecodedCommand(&command, fs));
    }

    fclose(fs);

    return EXIT_CODES::NO_ERRORS;
}
