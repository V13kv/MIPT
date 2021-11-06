#include <stdio.h>
#include <string.h>  // for strcpy && memcpy

#include "../../lib/text/include/text.h"

#include "../../include/asm/assembler.h"
#include "../../include/asm/labels.h"

#include "../../include/asm/settings.h"
#include "../../include/constants.h"

// TODO: 2 args support, strip command args

static EXIT_CODES parseCommand(text_line_t *line, command_t *command, labels_t *unprocCommandArgLabels, const int globalOffset);
static EXIT_CODES normalizeCodeLine(text_line_t *line);
static EXIT_CODES hasArguments(char *mnemonics, bool *hasArgs);
static EXIT_CODES isSpecialInstruction(command_t *command, bool *isSpecInstr);
static EXIT_CODES checkMnemonics(char *mnemonics);
static EXIT_CODES setCommandMnemonics(command_t *command, char *mnemonics);
static EXIT_CODES setCommandOpcode(command_t *command);
static EXIT_CODES parseCommandArguments(command_t *command, text_line_t *line, int argsStart, int argsEnd, labels_t *unprocCommandArgLabels, const int globalOffset);
static EXIT_CODES parseArgument(command_t *command, int *argNumber, text_line_t *line, int *argStart);
static EXIT_CODES checkRegisterForCorrectness(char *reg);
static EXIT_CODES getArgumentsMathOperation(text_line_t *line, int *argStart, char *mathOP);

static EXIT_CODES encodeCommand(command_t *command);
static EXIT_CODES encodeRegisterArgument(command_t *command, char *regStr);
static EXIT_CODES encodeImmediateArgument(command_t *command, char *immStr);
static EXIT_CODES exportEncodedCommand(command_t *command, FILE *fs);

static EXIT_CODES fillUnprocCommandArgLabels(labels_t *unproc, labels_t *labels, FILE *fs);

static EXIT_CODES resetCommand(command_t *command);

// ******TODO: Долгосрочная перспектива: система ошибок без exit'ов, всё возвращать в main
EXIT_CODES assembly(text_t *code, char *outputFileName)
{
    // Error check
    if (code == NULL || outputFileName == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::PASSED_OBJECT_IS_NULLPTR);
        return EXIT_CODES::PASSED_OBJECT_IS_NULLPTR;
    }

    // Output file
    FILE *fs = fopen(outputFileName, "wb");
    if (fs == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::BAD_STD_FUNC_RESULT);
        return EXIT_CODES::BAD_STD_FUNC_RESULT;
    }

    // General labels
    labels_t labels = {};
    IS_OK_W_EXIT(labelsCtor(&labels));

    // Command argument labels
    labels_t unprocCommandArgLabels = {};
    IS_OK_W_EXIT(labelsCtor(&unprocCommandArgLabels));

    // Assembly
    int globalOffset = 0;  // (local)
    command_t command = {};
    for (int line = 0; line < code->lines_count; ++line)
    {
        IS_OK_W_EXIT(normalizeCodeLine(&code->lines[line]));

        if (code->lines[line].length != 0)
        {
            printf("Wroking on line %s\n", code->lines[line].beginning);
            
            if (isLabel(code->lines[line].beginning, LABEL_LINE_FORMAT))
            {
                IS_OK_W_EXIT(initLabel(code->lines[line].beginning, &labels, LABEL_LINE_FORMAT, globalOffset));
            }
            else
            {
                // Parse command
                IS_OK_W_EXIT(parseCommand(&code->lines[line], &command, &unprocCommandArgLabels, globalOffset));
                printf(GREEN "\t[+] " RESET "Parsing\n");

                IS_OK_W_EXIT(encodeCommand(&command));
                printf(GREEN "\t[+] " RESET "Encoding\n");

                IS_OK_W_EXIT(exportEncodedCommand(&command, fs));
                printf(GREEN "\t[+] " RESET "Exporting\n");
                
                // Update global offset (for label's offset identification)
                globalOffset += command.encoded.bytes;

                IS_OK_W_EXIT(resetCommand(&command));  
            }
        }
    }
    
    IS_OK_W_EXIT(fillUnprocCommandArgLabels(&unprocCommandArgLabels, &labels, fs));
    printf(GREEN "[+] Translation successfully done\n" RESET);

    // for (int label = 0; label < labels.totalLabels; ++label)
    // {
    //     printf("#%d. Name: %s\nOffset: %ld\n", label, labels.labels[label].name, labels.labels[label].offset);
    // }

    IS_OK_W_EXIT(labelsDtor(&unprocCommandArgLabels));
    IS_OK_W_EXIT(labelsDtor(&labels));
    fclose(fs);

    return EXIT_CODES::NO_ERRORS;
}

static EXIT_CODES normalizeCodeLine(text_line_t *line)
{
    // Error check
    if (line == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::PASSED_OBJECT_IS_NULLPTR);
        return EXIT_CODES::PASSED_OBJECT_IS_NULLPTR;
    }

    // Beautifying
    char beautified[MAX_MNEMONICS_STR_LENGTH + MAX_INSTRUCTION_ARGS_STR_LEN] = {};
    int ret = sscanf(line->beginning, EXCLUDE_COMMENTS_FORMAT, beautified);
    CHECK_SSCANF_RESULT(ret);

    strcpy(line->beginning, stripLine(beautified));
    line->length = strlen(line->beginning);
    // strcpy(line->beginning, beautified);

    return EXIT_CODES::NO_ERRORS;
}

static EXIT_CODES parseCommand(text_line_t *line, command_t *command, labels_t *unprocCommandArgLabels, const int globalOffset)
{
    // Error check
    if (line == NULL || command == NULL || unprocCommandArgLabels == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::PASSED_OBJECT_IS_NULLPTR);
        return EXIT_CODES::PASSED_OBJECT_IS_NULLPTR;
    }

    // Get mnemonics && arguments
    int mnemonicsEnd    = 0;
    int argsStart       = 0;
    int argsEnd         = 0;
    char mnemonics[MAX_MNEMONICS_STR_LENGTH] = {};
    char arguments[MAX_ARGUMENTS_PER_COMMAND * MAX_ARGUMENT_STR_LENGTH] = {};

    int ret = sscanf(line->beginning, COMMAND_FORMAT, mnemonics, &mnemonicsEnd, &argsStart, arguments, &argsEnd);
    CHECK_SSCANF_RESULT(ret);
    if (ret == 0)
    {
        PRINT_ERROR_TRACING_MESSAGE(ASM_EXIT_CODES::BAD_COMMAND_FORMAT);
        return EXIT_CODES::BAD_OBJECT_PASSED;
    }

    // Parse mnemonics
    // TODO: посчитать один раз индекс нахождения в массиве
    IS_OK_W_EXIT(checkMnemonics(mnemonics)); 
    IS_OK_W_EXIT(setCommandMnemonics(command, mnemonics));
    IS_OK_W_EXIT(setCommandOpcode(command));

    // Check mnemonics for arguments existence
    // TODO: normal arguments check, normal command opdefs format etc. (do it via adding ',' support for 2+ arguments)
    bool hasArgs = true;
    IS_OK_W_EXIT(hasArguments(command->mnemonics, &hasArgs));

    // printf("command->mnemonics: %s; ", command->mnemonics);
    // printf("hashArguments: %d\n", hasArgs);
    if (hasArgs)
    {
        // Check for whitespace between mnemonics and arguments
        if (mnemonicsEnd == argsStart)
        {
            PRINT_ERROR_TRACING_MESSAGE(ASM_EXIT_CODES::BAD_COMMAND_FORMAT);
            return EXIT_CODES::BAD_OBJECT_PASSED;
        }

        // Parse arguments
        IS_OK_W_EXIT(parseCommandArguments(command, line, argsStart, argsEnd, unprocCommandArgLabels, globalOffset));
    }

    // printf("command->mnemonics: %s\n", command->mnemonics);
    // printf("command->opcode: %d\n", command->opcode);
    // printf("command->MRI: 0x%x\n", command->MRI);
    // printf("command->argumentsCount: %d\n", command->argumentsCount);
    // putchar('\n');

    return EXIT_CODES::NO_ERRORS;
}

static EXIT_CODES hasArguments(char *mnemonics, bool *hasArgs)
{
    // Erorr check
    if (mnemonics == NULL || hasArgs == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::PASSED_OBJECT_IS_NULLPTR);
        return EXIT_CODES::PASSED_OBJECT_IS_NULLPTR;
    }

    // Get argument count
    for (size_t instrIndx = 0; instrIndx < MNEMONICS_TABLE_LENGTH; ++instrIndx)
    {
        if (!strcmp(mnemonics, MNEMONICS_TABLE[instrIndx]))
        {
            *hasArgs = !!ARGS_COUNT_TABLE[instrIndx];
            return EXIT_CODES::NO_ERRORS;
        }
    }

    PRINT_ERROR_TRACING_MESSAGE(ASM_EXIT_CODES::UNKNOWN_MNEMONICS);
    return EXIT_CODES::BAD_OBJECT_PASSED;
}

static EXIT_CODES isSpecialInstruction(command_t *command, bool *isSpecInstr)
{
    // Error check
    if (command == NULL || isSpecInstr == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::PASSED_OBJECT_IS_NULLPTR);
        return EXIT_CODES::PASSED_OBJECT_IS_NULLPTR;
    }

    // Check
    if (!strcmp(command->mnemonics, "jmp"))
    {
        *isSpecInstr = true;
    }
    else if (!strcmp(command->mnemonics, "je"))
    {
        *isSpecInstr = true;
    }
    else if (!strcmp(command->mnemonics, "jne"))
    {
        *isSpecInstr = true;
    }
    else if (!strcmp(command->mnemonics, "jl"))
    {
        *isSpecInstr = true;
    }
    else if (!strcmp(command->mnemonics, "jg"))
    {
        *isSpecInstr = true;
    }
    else if (!strcmp(command->mnemonics, "call"))
    {
        *isSpecInstr = true;
    }
    else
    {
        *isSpecInstr = false;
    }

    return EXIT_CODES::NO_ERRORS;
}

static EXIT_CODES checkMnemonics(char *mnemonics)
{
    // Error check
    if (mnemonics == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::PASSED_OBJECT_IS_NULLPTR);
        return EXIT_CODES::PASSED_OBJECT_IS_NULLPTR;
    }

    // Cheking
    for (size_t instrIndx = 0; instrIndx < MNEMONICS_TABLE_LENGTH; ++instrIndx)
    {
        if (!strcmp(MNEMONICS_TABLE[instrIndx], mnemonics))
        {
            return EXIT_CODES::NO_ERRORS;  
        }
    }

    PRINT_ERROR_TRACING_MESSAGE(ASM_EXIT_CODES::UNKNOWN_MNEMONICS);
    return EXIT_CODES::BAD_OBJECT_PASSED;
}

static EXIT_CODES setCommandMnemonics(command_t *command, char *mnemonics)
{
    // Error check
    if (command == NULL || mnemonics == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::PASSED_OBJECT_IS_NULLPTR);
        return EXIT_CODES::PASSED_OBJECT_IS_NULLPTR;
    }

    // Set command mnemonics
    strcpy(command->mnemonics, mnemonics);

    return EXIT_CODES::NO_ERRORS;
}

static EXIT_CODES setCommandOpcode(command_t *command)
{
    // Error check
    if (command == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::PASSED_OBJECT_IS_NULLPTR);
        return EXIT_CODES::PASSED_OBJECT_IS_NULLPTR;
    }

    // Set command mnemonics opcode
    for (size_t mnemonics = 0; mnemonics < MNEMONICS_TABLE_LENGTH; ++mnemonics)
    {
        if (!strcmp(MNEMONICS_TABLE[mnemonics], command->mnemonics))
        {
            command->opcode = OPCODES_TABLE[mnemonics];
            return EXIT_CODES::NO_ERRORS;
        }
    }

    PRINT_ERROR_TRACING_MESSAGE(ASM_EXIT_CODES::UNKNOWN_MNEMONICS);
    return EXIT_CODES::BAD_OBJECT_PASSED;
}

static EXIT_CODES parseCommandArguments(command_t *command, text_line_t *line, int argsStart, int argsEnd, labels_t *unprocCommandArgLabels, const int globalOffset)
{
    // Error check
    if (command == NULL || line == NULL || unprocCommandArgLabels == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::PASSED_OBJECT_IS_NULLPTR);
        return EXIT_CODES::PASSED_OBJECT_IS_NULLPTR;
    }

    // Check for special instructions (jmp, call)
    bool isSpecInstr = true;
    IS_OK_W_EXIT(isSpecialInstruction(command, &isSpecInstr));

    if (isSpecInstr)
    {
        if (isLabel(&line->beginning[argsStart], LABEL_ARG_FORMAT))
        {
            IS_OK_W_EXIT(initLabel(&line->beginning[argsStart], unprocCommandArgLabels, LABEL_ARG_FORMAT, globalOffset));
            
            command->isSpecialCommand   = true;
            command->argumentsCount     = ONE_ARGUMENT;
        }
        else
        {
            PRINT_ERROR_TRACING_MESSAGE(ASM_EXIT_CODES::BAD_LABEL_FORMAT);
            return EXIT_CODES::BAD_OBJECT_PASSED;
        }
    }
    else
    {
        // Check for correct memory brackets
        if  ((line->beginning[argsStart] == '[' && line->beginning[argsEnd - 1] != ']') ||
             (line->beginning[argsStart] != '[' && line->beginning[argsEnd - 1] == ']'))
        {
            PRINT_ERROR_TRACING_MESSAGE(ASM_EXIT_CODES::BAD_COMMAND_MEMORY_BRACKETS_USE);
            return EXIT_CODES::BAD_OBJECT_PASSED;
        }

        // Set command MRI
        if (line->beginning[argsStart] == '[')
        {
            ++argsStart;
            --argsEnd;

            SET_MRI_MEMORY(command->MRI);
        }

        // Parse command arguments
        int argNumber = 0;
        char op = '\0';
        int argStart = argsStart;
        while (argStart < argsEnd)
        {
            IS_OK_W_EXIT(parseArgument(command, &argNumber, line, &argStart));
            if (argStart < argsEnd)
            {
                // TODO: Support of -, etc (*additional all math ops as functions, like +(ax, 123) etc)
                IS_OK_W_EXIT(getArgumentsMathOperation(line, &argStart, &op));
            }
        }
        command->argumentsCount = argNumber;
    }
    
    return EXIT_CODES::NO_ERRORS;
}

static EXIT_CODES parseArgument(command_t *command, int *argNumber, text_line_t *line, int *argStart)
{
    // Error check
    if (command == NULL || argNumber == NULL || line == NULL || argStart == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::PASSED_OBJECT_IS_NULLPTR);
        return EXIT_CODES::PASSED_OBJECT_IS_NULLPTR;
    }

    // Parse one argument
    int newArgStart = 0;
    char reg[MAX_REGISTER_STR_LENGTH] = {};
    if (sscanf(&line->beginning[*argStart], REGISTER_FORMAT, reg, &newArgStart) == 1)
    {
        IS_OK_W_EXIT(checkRegisterForCorrectness(reg));
        strcpy(command->arguments[*argNumber], reg);

        SET_MRI_REGISTER(command->argsMRI[*argNumber]);
    }
    else
    {
        double imm = 0;
        if (sscanf(&line->beginning[*argStart], IMMEDIATE_VALUE_FORMAT, &imm, &newArgStart) == 1)
        {
            memcpy(command->arguments[*argNumber], &imm, sizeof(double));

            SET_MRI_IMMEDIATE(command->argsMRI[*argNumber]);
        }
        else
        {
            PRINT_ERROR_TRACING_MESSAGE(ASM_EXIT_CODES::BAD_COMMAND_ARGUMENTS);
            return EXIT_CODES::BAD_OBJECT_PASSED;
        }
    }

    *argStart += newArgStart;
    ++(*argNumber);

    return EXIT_CODES::NO_ERRORS;
}

static EXIT_CODES checkRegisterForCorrectness(char *reg)
{
    // Error check
    if (reg == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::PASSED_OBJECT_IS_NULLPTR);
        return EXIT_CODES::PASSED_OBJECT_IS_NULLPTR;
    }

    // Check
    if (!strcmp(reg, "ax"))
    {
        return EXIT_CODES::NO_ERRORS;
    }
    else if (!strcmp(reg, "bx"))
    {
        return EXIT_CODES::NO_ERRORS;
    }
    else if (!strcmp(reg, "cx"))
    {
        return EXIT_CODES::NO_ERRORS;
    }
    else if (!strcmp(reg, "dx"))
    {
        return EXIT_CODES::NO_ERRORS;
    }
    else
    {
        PRINT_ERROR_TRACING_MESSAGE(ASM_EXIT_CODES::UNKNOWN_COMMAND_REGISTER);
        return EXIT_CODES::BAD_OBJECT_PASSED;
    }

    // for (size_t regName = 0; regName < REGISTERS_TABLE_LENGTH; ++regName)
    // {
    //     if (!strcmp(reg, REGISTERS_TABLE[regName].name))
    //     {
    //         return EXIT_CODES::NO_ERRORS;
    //     }
    // }
}

// TODO: Support of -, etc (*additional all math ops as functions, like +(ax, 123) etc)
static EXIT_CODES getArgumentsMathOperation(text_line_t *line, int *argStart, char *mathOP)
{
    // Error check
    if (mathOP == NULL || line == NULL || argStart == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::PASSED_OBJECT_IS_NULLPTR);
        return EXIT_CODES::PASSED_OBJECT_IS_NULLPTR;
    }

    // Get math operation
    switch (line->beginning[*argStart])
    {
        case '+':
            *mathOP = '+';
            break;
        case '-':
            *mathOP = '-';
            break;
        default:
            *mathOP = '\0';
            PRINT_ERROR_TRACING_MESSAGE(ASM_EXIT_CODES::BAD_MATH_OPERATION_IN_COMMAND_ARG);
            return EXIT_CODES::BAD_OBJECT_PASSED;
    }
    ++(*argStart);

    return EXIT_CODES::NO_ERRORS;
}


static EXIT_CODES encodeCommand(command_t *command)
{
    // Error check
    if (command == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::PASSED_OBJECT_IS_NULLPTR);
        return EXIT_CODES::PASSED_OBJECT_IS_NULLPTR;
    }

    // Encode metadata
    command->encoded.byteData[command->encoded.bytes++] = (byte) command->opcode;

    // Special instructions encoding
    if (command->isSpecialCommand)
    {
        command->encoded.bytes += sizeof(offset);
    }
    // Common (not zero arg) instructions encoding
    else if (command->argumentsCount != NO_ARGUMENTS)
    {
        // Encode metadata
        command->encoded.byteData[command->encoded.bytes] = (byte) ENCODE_COMMAND_ARGS_COUNT(command->argumentsCount);
        command->encoded.byteData[command->encoded.bytes++] |= (byte) ENCODE_COMMAND_MRI(command->MRI);

        // Encode arguments
        for (size_t arg = 0; arg < command->argumentsCount; ++arg)
        {
            // Encode arg's MRI
            command->encoded.byteData[command->encoded.bytes++] = (byte) command->argsMRI[arg];
            
            // Properly encode argument
            if (ARG_IS_REGISTER(command->argsMRI[arg]))
            {
                IS_OK_W_EXIT(encodeRegisterArgument(command, command->arguments[arg]));
                command->encoded.bytes += sizeof(byte);
            }
            else
            {
                IS_OK_W_EXIT(encodeImmediateArgument(command, command->arguments[arg]));
                command->encoded.bytes += sizeof(double);
            }
        }
    }

    return EXIT_CODES::NO_ERRORS;
}

static EXIT_CODES encodeRegisterArgument(command_t *command, char *regStr)
{
    // Error check
    if (command == NULL || regStr == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::PASSED_OBJECT_IS_NULLPTR);
        return EXIT_CODES::PASSED_OBJECT_IS_NULLPTR;
    }

    // Encode
    if (!strcmp(regStr, "ax"))
    {
        command->encoded.byteData[command->encoded.bytes] = 0;  // ax opcode
        return EXIT_CODES::NO_ERRORS;
    }
    else if (!strcmp(regStr, "bx"))
    {
        command->encoded.byteData[command->encoded.bytes] = 1;  // bx opcode
        return EXIT_CODES::NO_ERRORS;
    }
    else if (!strcmp(regStr, "cx"))
    {
        command->encoded.byteData[command->encoded.bytes] = 2;  // cx opcode
        return EXIT_CODES::NO_ERRORS;
    }
    else if (!strcmp(regStr, "dx"))
    {
        command->encoded.byteData[command->encoded.bytes] = 3;  // dx opcode
        return EXIT_CODES::NO_ERRORS;
    }
    else
    {
        PRINT_ERROR_TRACING_MESSAGE(ASM_EXIT_CODES::UNKNOWN_COMMAND_REGISTER);
        return EXIT_CODES::BAD_OBJECT_PASSED;
    }

    // for (size_t reg = 0; reg < REGISTERS_TABLE_LENGTH; ++reg)
    // {
    //     if (!strcmp(regStr, REGISTERS_TABLE[reg].name))
    //     {
    //         command->encoded.byteData[command->encoded.bytes] = (byte) REGISTERS_TABLE[reg].opcode;
    //         return EXIT_CODES::NO_ERRORS;
    //     }
    // }

    // PRINT_ERROR_TRACING_MESSAGE(ASM_EXIT_CODES::UNKNOWN_COMMAND_REGISTER);
    // return EXIT_CODES::BAD_OBJECT_PASSED;
}

static EXIT_CODES encodeImmediateArgument(command_t *command, char *immStr)
{
    // Error check
    if (command == NULL || immStr == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::PASSED_OBJECT_IS_NULLPTR);
        return EXIT_CODES::PASSED_OBJECT_IS_NULLPTR;
    }

    // Encode
    memcpy(&command->encoded.byteData[command->encoded.bytes], immStr, sizeof(double));

    return EXIT_CODES::NO_ERRORS;
}

static EXIT_CODES exportEncodedCommand(command_t *command, FILE *fs)
{
    // Error check
    if (command == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::PASSED_OBJECT_IS_NULLPTR);
        return EXIT_CODES::PASSED_OBJECT_IS_NULLPTR;
    }
    
    // Export
    fwrite(command->encoded.byteData, sizeof(byte), command->encoded.bytes, fs);

    return EXIT_CODES::NO_ERRORS;
}


static EXIT_CODES fillUnprocCommandArgLabels(labels_t *unproc, labels_t *labels, FILE *fs)
{
    // Error check
    if (unproc == NULL || labels == NULL || fs == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::PASSED_OBJECT_IS_NULLPTR);
        return EXIT_CODES::PASSED_OBJECT_IS_NULLPTR;
    }

    // Fill unprocessed argument labels of commands
    for (int argLabel = 0; argLabel < unproc->totalLabels; ++argLabel)
    {
        for (int label = 0; label < labels->totalLabels; ++label)
        {
            if (!strcmp(unproc->labels[argLabel].name, labels->labels[label].name))
            {
                CHECK_FSEEK_RESULT(fseek(fs, unproc->labels[argLabel].offset + 1, SEEK_SET));

                fwrite(&labels->labels[label].offset, sizeof(offset), 1, fs);
            }
        }
    }

    return EXIT_CODES::NO_ERRORS;
}


static EXIT_CODES resetCommand(command_t *command)
{
    // Erorr check
    if (command == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::PASSED_OBJECT_IS_NULLPTR);
        return EXIT_CODES::PASSED_OBJECT_IS_NULLPTR;
    }

    // Reset
    memset(command->mnemonics, 0, sizeof(command->mnemonics));
    memset(command->arguments, 0, sizeof(command->arguments));
    memset(command->argsMRI, 0, sizeof(command->argsMRI));
    memset(command->encoded.byteData, 0, sizeof(command->encoded.byteData));

    command->opcode             = 0;
    command->argumentsCount     = 0;
    command->MRI                = 0;
    command->isSpecialCommand   = 0;
    command->encoded.bytes      = 0;

    return EXIT_CODES::NO_ERRORS;
}
