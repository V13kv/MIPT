#include <stdio.h>  // for sscanf
#include <string.h>  // for memset

//TODO: compiler flag (убрать ../../)
#include "../../include/asm_general.h"
#include "../../include/myasm/assembler.h"
#include "../../include/commands.h"

// TODO: encode jmp properly, call instructions (separate labels and register names)
EXIT_CODES encodeCommand(command_t *const command, char *encodedCommand)
{
    // Error check
    if (command == NULL || encodedCommand == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::PASSED_OBJECT_IS_NULLPTR);
        return EXIT_CODES::PASSED_OBJECT_IS_NULLPTR;
    }

    // Encoding 1st instr header byte (opcode && arguments_count)
    int encodingByte = 0;

    unsigned int instrHeader1       = ENCODE_COMMAND_OPCODE(command->opcode);
    instrHeader1                    = instrHeader1 | ENCODE_COMMAND_ARGS_COUNT(command->arguments_count);
    encodedCommand[encodingByte++]  = (char) instrHeader1;

    // Encoding 2nd instr header byte (MRI (mem, reg, imm))
    unsigned int instrHeader2       = ENCODE_COMMAND_MRI(command->MRI);
    encodedCommand[encodingByte++]  = (char) instrHeader2;

    // Encoding command arguments
    if (MRI_HAS_REGISTER(command->MRI))
    {
        int regOpcode = -1;
        IS_OK_W_EXIT(getCommandArgRegisterOpcode(command, &regOpcode));
        
        encodedCommand[encodingByte++] = (char) regOpcode;
    }
    
    if (MRI_HAS_IMMEDIATE(command->MRI))
    {
        double imm;
        IS_OK_W_EXIT(getCommandArgImmValue(command, &imm));

        char *cImm = (char *) &imm;
        for (int byte = 0; byte < (int) sizeof(double); ++byte)
        {
            encodedCommand[encodingByte++] = *cImm++;
        }
    }
    command->bytes = encodingByte;

    // printf("command->mnemonics: %s\n", command->mnemonics);
    // printf("command->opcode: %d\n", command->opcode);
    // printf("command->MRI: 0x%x\n", (unsigned int) command->MRI);
    // printf("command->arguments_count: %d\n", command->arguments_count);
    // printf("command->arguments: %s\n", command->arguments);
    // printf("command->bytes: %d\n\n", command->bytes);

    return EXIT_CODES::NO_ERRORS;
}

EXIT_CODES exportEncodedCommand(char *encodedCommand, int bytesToExport, FILE *fs)
{
    // Error check
    if (encodedCommand == NULL || fs == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::PASSED_OBJECT_IS_NULLPTR);
        return EXIT_CODES::PASSED_OBJECT_IS_NULLPTR;
    }

    // Export
    fwrite(encodedCommand, sizeof(char), bytesToExport, fs);

    return EXIT_CODES::NO_ERRORS;
}

// TODO: support of two arguments separated ','
EXIT_CODES parseLine(text_line_t *line, command_t *command)
{
    // Error check
    if (line == NULL || command == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::BAD_OBJECT_PASSED);
        return EXIT_CODES::BAD_OBJECT_PASSED;
    }

    // Parse line
    int ret = sscanf(line->beginning, "%s %s;", command->mnemonics, command->arguments);
    if (ret == 0 || ret == EOF)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::BAD_STD_FUNC_RESULT);
        return EXIT_CODES::BAD_STD_FUNC_RESULT;
    }
    
    // Fill `command` structure with contents of the current `line`
    IS_OK_WO_EXIT(getMnemonicsOpcode(command->mnemonics, &command->opcode));
    IS_OK_WO_EXIT(getCommandMRI(command, &command->MRI));
    command->arguments_count = GET_ARGS_COUNT_VIA_MRI(command->MRI);

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

    // Output binary file
    FILE *fs = fopen(output_file_name, "wb");
    if (fs == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::BAD_STD_FUNC_RESULT);
        return EXIT_CODES::BAD_STD_FUNC_RESULT;
    }

    // Parse each source code line
    command_t command = {};
    char encodedCommand[MAX_ENCODED_COMMAND_LENGTH] = {};
    for (int line = 0; line < text->lines_count; ++line)
    {
        IS_OK_W_EXIT(parseLine(&text->lines[line], &command));
        IS_OK_W_EXIT(encodeCommand(&command, encodedCommand));       
        IS_OK_W_EXIT(exportEncodedCommand(encodedCommand, command.bytes, fs));
      
        IS_OK_W_EXIT(resetCommand(&command));
        memset(encodedCommand, 0, sizeof(encodedCommand));
    }

    fclose(fs);

    return EXIT_CODES::NO_ERRORS;
}
