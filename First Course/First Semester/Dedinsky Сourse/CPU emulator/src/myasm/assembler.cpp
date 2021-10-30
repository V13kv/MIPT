#include <stdio.h>  // for sscanf
#include <string.h>  // for strcmp && strcpy
#include <stdbool.h>
#include <stdlib.h> // for calloc && free

//TODO: compiler flag (убрать ../../)
#include "../../include/myasm/assembler.h"
#include "../../include/registers.h"
#include "../../include/commands.h"

// TODO: encode jmp properly, call instructions (separate labels and register names)
EXIT_CODES encodeCommand(command_t *const command, char *const encodedCommand)
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

    return EXIT_CODES::NO_ERRORS;
}

EXIT_CODES exportEncodedCommand(const char *const encodedCommand, const int bytesToExport, FILE *fs)
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

bool registerNameIsCorrect(char reg[])
{
    // TODO: Error check

    // Check
    for (int i = 0; i < sizeof(registers); ++i)
    {
        if (!strcmp(reg, registers[i].name))
        {
            return true;
        }
    }

    return false;
}

EXIT_CODES parseCommandArgs(const text_line_t *const line, int *argInd, int *strPos, command_t *const command)
{
    // TODO: error check

    // Parsing
    int savedStrPos = *strPos;

    double imm = -1;
    char reg[MAX_REG_STR_LENGTH + 1] = {};
    if (sscanf(&line->beginning[*strPos], "%2[a-z]%n", reg, strPos) == 1)
    {
        if (!registerNameIsCorrect(reg))
        {
            // TODO: add COMMAND_EXIT_CODES::BAD_REGISTER_NAME
            PRINT_ERROR_TRACING_MESSAGE(COMMAND_EXIT_CODES::WRONG_COMMAND_FORMAT);
            return EXIT_CODES::BAD_OBJECT_PASSED;
        }

        strcpy(command->arguments[*argInd], reg);
    }
    else if (sscanf(&line->beginning[*strPos], "%lf%n", &imm, strPos) == 1)
    {
        char cImm[5] = {};
        snprintf(cImm, 5, "%lf", imm);

        strcpy(command->arguments[*argInd], cImm);
    }
    else 
    {
        PRINT_ERROR_TRACING_MESSAGE(COMMAND_EXIT_CODES::WRONG_COMMAND_FORMAT);
        return EXIT_CODES::BAD_OBJECT_PASSED;
    }
    *strPos += savedStrPos;
    ++*argInd;

    return EXIT_CODES::NO_ERRORS;
}

// TODO: support of two arguments separated ','
EXIT_CODES parseCommand(const text_line_t *const line, command_t *const command)//, char **labels)
{
    // Error check
    if (line == NULL || command == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::BAD_OBJECT_PASSED);
        return EXIT_CODES::BAD_OBJECT_PASSED;
    }

    // ----------------------PARSING----------------------
    // Get mnemonics
    int strPos = 0;
    if (sscanf(line->beginning, "%[a-zA-Z] %n", command->mnemonics, &strPos) != 1)
    {
        PRINT_ERROR_TRACING_MESSAGE(COMMAND_EXIT_CODES::WRONG_COMMAND_FORMAT);
        return EXIT_CODES::BAD_OBJECT_PASSED;
    }

    // if (!strcmp(command->mnemonics, commands_table[COMMAND_OPCODES::JMP]))
    // {
        
    // }

    // Parse arguments
    if (line->beginning[strPos] == '[')
    {
        SET_MRI_MEMORY(command->MRI);
        strPos++;
    }

    int argInd = 0;
    while (strPos < line->length)
    {
        IS_OK_W_EXIT(parseCommandArgs(line, &argInd, &strPos, command));

        if (line->beginning[strPos] == '+')
        {
            strPos++;
        }
        else if (line->beginning[strPos] == ']')
        {
            break;
        }
    }

    if (MRI_HAS_MEMORY(command->MRI) && !(line->beginning[strPos] == ']'))
    {
        PRINT_ERROR_TRACING_MESSAGE(COMMAND_EXIT_CODES::WRONG_COMMAND_FORMAT);
        return EXIT_CODES::BAD_OBJECT_PASSED;
    }

    // Fill `command` structure with contents of the current `line`
    IS_OK_WO_EXIT(getMnemonicsOpcode(command->mnemonics, &command->opcode));
    IS_OK_WO_EXIT(getCommandMRI(command, &command->MRI));
    command->arguments_count = argInd;

    // printf("command->mnemonics: %s\n", command->mnemonics);
    // printf("command->opcode: %d\n", command->opcode);
    // printf("command->MRI: 0x%x\n", (unsigned int) command->MRI);
    // printf("command->arguments_count: %d\n", command->arguments_count);
    // for (int i = 0; i < command->arguments_count; ++i)
    // {
    //     if (command->arguments[i])
    //     {
    //         printf("#%d: %s\n", i, command->arguments[i]);
    //     }
    // }
    // printf("\n");

    return EXIT_CODES::NO_ERRORS;
}

// EXIT_CODES getLabel(const text_line_t *const line, char **labels)
// {
//     // TODO: Error check

//     // Get label
//     int strPos = 0;
//     char label[MAX_LABEL_STR_LENGTH] = {};
//     sscanf(line->beginning, "%[a-zA-Z0-9_]:%n", label, &strPos);
// }

EXIT_CODES assembly(const text_t *const code, const char *const output_file_name)
{
    // Error check
    if (code == NULL || code->lines == NULL || output_file_name == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::BAD_OBJECT_PASSED);
        return EXIT_CODES::BAD_OBJECT_PASSED;
    }
    for (int line = 0; line < code->lines_count; ++line)
    {
        if (code->lines[line].beginning == NULL)
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

    // char **labels = (char **) calloc(MIN_LABELS_COUNT, sizeof(char *));
    // for (int label = 0; label < MIN_LABELS_COUNT; ++label)
    // {
    //     labels[label] = (char *) calloc(1, MAX_LABEL_STR_LENGTH);
    // }
    // for (int line = 0; line < code->lines_count; ++line)
    // {
    //     IS_OK_W_EXIT(getLabel(&code->lines[line], labels));
    // }

    // Parse each source code line
    command_t command = {};
    char encodedCommand[MAX_ENCODED_COMMAND_LENGTH] = {};
    for (int line = 0; line < code->lines_count; ++line)
    {
        IS_OK_W_EXIT(parseCommand(&code->lines[line], &command));//, labels));
        IS_OK_W_EXIT(encodeCommand(&command, encodedCommand));       
        
        IS_OK_W_EXIT(exportEncodedCommand(encodedCommand, command.bytes, fs));
      
        IS_OK_W_EXIT(resetCommand(&command));
    }

    // for (int label = 0; label < MIN_LABELS_COUNT; ++label)
    // {
    //     free(labels[label]);
    // }
    // free(labels);  

    fclose(fs);

    return EXIT_CODES::NO_ERRORS;
}
