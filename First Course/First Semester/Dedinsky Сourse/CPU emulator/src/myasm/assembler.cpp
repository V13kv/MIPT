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
    //TODO: multiple registers support
    if (MRI_HAS_REGISTER(command->MRI))
    {
        int regOpcode = (int) REGISTER_OPCODES::INVALID_OPCODE;
        IS_OK_W_EXIT(getCommandArgRegisterOpcode(command, &regOpcode));
        
        encodedCommand[encodingByte++] = (char) regOpcode;
    }
    
    if (MRI_HAS_IMMEDIATE(command->MRI))
    {
        //TODO: multiple imm's support
        IS_OK_W_EXIT(encodeCommandArgImmValue(command, encodedCommand, &encodingByte));
    }
    command->bytes = encodingByte;

    if (command->opcode == (int) COMMAND_OPCODES::JMP)
    {
        command->bytes += sizeof(lb.offset);
    }

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

EXIT_CODES parseCommandArg(const text_line_t *const line, int *argInd, int *strPos, command_t *const command)
{
    // Error check
    if (line == NULL || argInd == NULL || strPos == NULL || command == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::PASSED_OBJECT_IS_NULLPTR);
        return EXIT_CODES::PASSED_OBJECT_IS_NULLPTR;
    }

    // Parsing
    int savedStrPos = *strPos;

    double imm = -1;
    char reg[MAX_REG_STR_LENGTH + 1] = {};
    if (sscanf(&line->beginning[*strPos], "%2[a-z]%n", reg, strPos) == 1)
    {
        if (!registerNameIsCorrect(reg))
        {
            PRINT_ERROR_TRACING_MESSAGE(COMMAND_EXIT_CODES::BAD_REGISTER_NAME);
            return EXIT_CODES::BAD_OBJECT_PASSED;
        }
        strcpy(command->arguments[*argInd], reg);

        SET_MRI_REGISTER(command->MRI);
    }
    else if (sscanf(&line->beginning[*strPos], "%lf%n", &imm, strPos) == 1)
    {
        char *cImm = (char *) &imm;
        for (int byte = 0; byte < (int) sizeof(double); ++byte)
        {
            command->arguments[*argInd][byte] = *cImm++;
        }

        SET_MRI_IMMEDIATE(command->MRI);
    }
    else 
    {
        PRINT_ERROR_TRACING_MESSAGE(COMMAND_EXIT_CODES::WRONG_COMMAND_FORMAT);
        return EXIT_CODES::BAD_OBJECT_PASSED;
    }
    *strPos += savedStrPos;
    *argInd += 1;

    return EXIT_CODES::NO_ERRORS;
}

EXIT_CODES parseCommandArgs(const text_line_t *const line, command_t *const command, int *strPos)
{
    // Error check
    if (line == NULL || command == NULL || strPos == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::PASSED_OBJECT_IS_NULLPTR);
        return EXIT_CODES::PASSED_OBJECT_IS_NULLPTR;
    }

    // Parsing
    int argInd = 0;
    while (*strPos < line->length)
    {
        IS_OK_W_EXIT(parseCommandArg(line, &argInd, strPos, command));

        if (line->beginning[*strPos] == '+')
        {
            *strPos += 1;
        }
        else if (line->beginning[*strPos] == ']')
        {
            break;
        }
    }
    command->arguments_count = argInd;

    return EXIT_CODES::NO_ERRORS;
}

bool isLabel(const text_line_t *const line)
{
    // Error check
    if (line == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::PASSED_OBJECT_IS_NULLPTR);
        return false;
    }

    int labelLength = 0;
    char label[MAX_LABEL_STR_LENGTH] = {};
    if (sscanf(line->beginning, "%[a-zA-Z0-9_]:%n", label, &labelLength))
    {
        if (labelLength != 0)
        {
            return true;
        }
    }

    return false;
}

// EXIT_CODES setLabelOffset(const text_line_t *const line, label_t labels[], long long int *globalOffset)
// {
//     // Error check
//     if (line == NULL || labels == NULL)
//     {
//         PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::PASSED_OBJECT_IS_NULLPTR);
//         return EXIT_CODES::PASSED_OBJECT_IS_NULLPTR;
//     }
//     // Setting
//     for (int label = 0; label < MAX_LABEL_OBJECTS; ++label)
//     {
//         if (!strcmp(line->beginning, labels[label].name))
//         {
//             if (labels[label].offset == -1)
//             {
//                 labels[label].offset = *globalOffset;
//                 return EXIT_CODES::NO_ERRORS;
//             }
//             else
//             {
//                 PRINT_ERROR_TRACING_MESSAGE(ASM_EXIT_CODES::MULTIPLE_LABELS_WITH_SAME_NAME);
//                 return EXIT_CODES::BAD_OBJECT_PASSED;
//             }
//         }
//     }
//     PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::BAD_OBJECT_PASSED);
//     return EXIT_CODES::BAD_OBJECT_PASSED;
// }

EXIT_CODES setCommandArgLabel(command_t *const command, char labelName[], label_t labels[])
{
    // Error check
    if (command == NULL || labels == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::PASSED_OBJECT_IS_NULLPTR);
        return EXIT_CODES::PASSED_OBJECT_IS_NULLPTR;
    }

    // Set label
    printf("labelName: %s\n", labelName);
    for (int label = 0; label < sizeof(labels); ++label)
    {
        if (!strcmp(labelName, labels[label].name))
        {
            char *cLabel = (char *) labels[label].offset;
            for (int byte = 0; byte < labels[label].length; ++byte)
            {
                command->arguments[0][byte] = *cLabel++;
            }

            return EXIT_CODES::NO_ERRORS;
        }
    }

    PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::BAD_OBJECT_PASSED);
    return EXIT_CODES::BAD_OBJECT_PASSED;
}

// Support of two arguments separated ','
EXIT_CODES parseCommand(const text_line_t *const line, command_t *const command, label_t labels[], PARSING_MODE mode)
{
    // Error check
    if (line == NULL || command == NULL || labels == NULL)
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

    IS_OK_WO_EXIT(getMnemonicsOpcode(command->mnemonics, &command->opcode));

    // Check for special instructions (jmp, TODO: call)
    if (!strcmp(command->mnemonics, commands_table[(int) COMMAND_OPCODES::JMP].mnemonics) && (bool) mode)
    {
        char commandLabelName[MAX_LABEL_STR_LENGTH] = {};
        if (sscanf(&line->beginning[strPos], "%[a-zA-Z0-9_]", commandLabelName) != 1)
        {
            PRINT_ERROR_TRACING_MESSAGE(ASM_EXIT_CODES::BAD_LABEL_NAME);
            return EXIT_CODES::BAD_OBJECT_PASSED;
        }
        printf("command->mnemonics: %s\n", command->mnemonics);
        IS_OK_W_EXIT(setCommandArgLabel(command, commandLabelName, labels));
    }
    else
    {
        // Get arguments
        if (line->beginning[strPos] == '[')
        {
            SET_MRI_MEMORY(command->MRI);
            ++strPos;
        }

        IS_OK_W_EXIT(parseCommandArgs(line, command, &strPos));
        
        if ( (!MRI_HAS_MEMORY(command->MRI) &&   line->beginning[strPos] == ']'  ) ||
            ( MRI_HAS_MEMORY(command->MRI) && !(line->beginning[strPos] == ']') ))
        {
            PRINT_ERROR_TRACING_MESSAGE(COMMAND_EXIT_CODES::WRONG_COMMAND_FORMAT);
            return EXIT_CODES::BAD_OBJECT_PASSED;
        }
    } 

    // IS_OK_WO_EXIT(getCommandMRI(command, &command->MRI)); 
    // printf("command->mnemonics: %s\n", command->mnemonics);
    // printf("command->opcode: %d\n", command->opcode);
    // printf("command->MRI: 0x%x\n", (unsigned int) command->MRI);
    // printf("command->arguments_count: %d\n", command->arguments_count);
    // // for (int i = 0; i < command->arguments_count; ++i)
    // // {
    // //     printf("#%d: %s\n", i, command->arguments[i]);
    // // }
    // // printf("\n");

    return EXIT_CODES::NO_ERRORS;
}

EXIT_CODES initLabel(const text_line_t *const line, label_t labels[], int *labelNumber, long long int globalOffset)
{
    // Error check
    if (line == NULL || labels == NULL || labelNumber == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::PASSED_OBJECT_IS_NULLPTR);
        return EXIT_CODES::PASSED_OBJECT_IS_NULLPTR;
    }

    // Get label
    int labelLength = 0;
    sscanf(line->beginning, "%[a-zA-Z0-9_]:%n", labels[*labelNumber].name, &labelLength);
    if (labelLength > MAX_LABEL_STR_LENGTH)
    {
        PRINT_ERROR_TRACING_MESSAGE(ASM_EXIT_CODES::LABEL_SIZE_IS_TOO_BIG);
        return EXIT_CODES::BAD_OBJECT_PASSED;
    }
    else
    {
        labels[*labelNumber].length = --labelLength;
        labels[*labelNumber].offset = globalOffset;
        *labelNumber += 1;
    }

    return EXIT_CODES::NO_ERRORS;
}

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

    // -----------------------------------SETUP LABELS-----------------------------------
    // TODO: первый проход по файлу: узнать все оффсеты всех меток
    long long int globalOffset = 0;
    
    command_t command = {};
    char encodedCommand[MAX_ENCODED_COMMAND_LENGTH] = {};
    
    int labelNumber = 0;
    label_t labels[MAX_LABEL_OBJECTS] = {};
    for (int line = 0; line < code->lines_count; ++line)
    {
        if (isLabel(&code->lines[line]))
        {
            IS_OK_W_EXIT(initLabel(&code->lines[line], labels, &labelNumber, globalOffset));
        }
        else
        {
            IS_OK_W_EXIT(parseCommand(&code->lines[line], &command, labels, PARSING_MODE::LABEL_PARSING));
            IS_OK_W_EXIT(encodeCommand(&command, encodedCommand));  
            globalOffset += command.bytes;

            IS_OK_W_EXIT(resetCommand(&command));
        }
    }

    for (int i = 0; i < labelNumber; ++i)
    {
        printf("#%d: \n\tname: %s\n\toffset: %d\n", i+1, labels[i].name, labels[i].offset);
    }

    // -----------------------------------ASSEMBLY SOURCE CODE-----------------------------------
    // TODO: второй проход по файлу: пройтись по всем КОМАНДАМ и за-encod-ить, используя высчитанные offset'ы меток
    for (int line = 0; line < code->lines_count; ++line)
    {
        if (!isLabel(&code->lines[line]))
        {
            IS_OK_W_EXIT(parseCommand(&code->lines[line], &command, labels, PARSING_MODE::COMMAND_PARSING));
            IS_OK_W_EXIT(encodeCommand(&command, encodedCommand));       
            IS_OK_W_EXIT(exportEncodedCommand(encodedCommand, command.bytes, fs));
        
            IS_OK_W_EXIT(resetCommand(&command));
        }   
    }

    fclose(fs);

    return EXIT_CODES::NO_ERRORS;
}
