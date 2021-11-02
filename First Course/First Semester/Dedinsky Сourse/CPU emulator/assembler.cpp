#include <stdio.h>
#include <string.h>  // for strcpy && memcpy

#include "include/asm/assembler.h"
#include "lib/text/include/text.h"
#include "include/asm/labels.h"

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

    // Initialization of needed objects
    labels_t labels = {};
    IS_OK_W_EXIT(labelsCtor(&labels));

    // Assembly
    command_t command = {};
    for (int line = 0; line < code->lines_count; ++line)
    {
        if (isLabel(&code->lines[line]))
        {
            IS_OK_W_EXIT(initLabel(&code->lines[line], &labels));
        }
        else
        {
            IS_OK_W_EXIT(parseCommand(&code->lines[line], &command));
            // encodeCommand();

            // exportEncodedCommand();
        }
        break;
    }

    IS_OK_W_EXIT(labelsDtor(&labels));

    return EXIT_CODES::NO_ERRORS;
}

EXIT_CODES parseCommand(text_line_t *line, command_t *command)
{
    // Error check
    if (line == NULL || command == NULL)
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

    // Check for whitespace between mnemonics and arguments
    if (mnemonicsEnd == argsStart)
    {
        PRINT_ERROR_TRACING_MESSAGE(ASM_EXIT_CODES::BAD_COMMAND_FORMAT);
        return EXIT_CODES::BAD_OBJECT_PASSED;
    }

    // Parse mnemonics
    IS_OK_W_EXIT(checkMnemonics(mnemonics)); 
    IS_OK_W_EXIT(setCommandMnemonics(command, mnemonics));
    IS_OK_W_EXIT(setCommandOpcode(command));

    // Parse arguments
    IS_OK_W_EXIT(parseCommandArguments(command, line, argsStart, argsEnd));

    printf("command->mnemonics: %s\n", command->mnemonics);
    printf("command->opcode: %d\n", command->opcode);
    printf("command->MRI: 0x%x\n", command->MRI);
    printf("command->argumentsCount %d\n", command->argumentsCount);
    for (int arg = 0; arg < command->argumentsCount; ++arg)
    {
        printf("argMRI: 0x%x; arg: %s\n", command->argsMRI[arg], command->arguments[arg]);
    }

    return EXIT_CODES::NO_ERRORS;
}

EXIT_CODES checkMnemonics(char *mnemonics)
{
    // Error check
    if (mnemonics == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::PASSED_OBJECT_IS_NULLPTR);
        return EXIT_CODES::PASSED_OBJECT_IS_NULLPTR;
    }

    // Cheking
    for (int instrName = 0; instrName < MNEMONICS_TABLE_LENGTH; ++instrName)
    {
        if (!strcmp(MNEMONICS_TABLE[instrName], mnemonics))
        {
            return EXIT_CODES::NO_ERRORS;  
        }
    }

    PRINT_ERROR_TRACING_MESSAGE(ASM_EXIT_CODES::UNKNOWN_MNEMONICS);
    return EXIT_CODES::BAD_OBJECT_PASSED;
}

EXIT_CODES setCommandMnemonics(command_t *command, char *mnemonics)
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

EXIT_CODES setCommandOpcode(command_t *command)
{
    // Error check
    if (command == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::PASSED_OBJECT_IS_NULLPTR);
        return EXIT_CODES::PASSED_OBJECT_IS_NULLPTR;
    }

    // Set command mnemonics opcode
    for (int mnemonics = 0; mnemonics < MNEMONICS_TABLE_LENGTH; ++mnemonics)
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

EXIT_CODES parseCommandArguments(command_t *command, text_line_t *line, const int argsStart, const int argsEnd)
{
    // Error check
    if (command == NULL || line == NULL || argsEnd == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::PASSED_OBJECT_IS_NULLPTR);
        return EXIT_CODES::PASSED_OBJECT_IS_NULLPTR;
    }

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
        SET_MRI_MEMORY(command->MRI);
    }

    // Parse command arguments
    int argEnd = 0;
    int argNumber = 0;
    while (argEnd != argsEnd)
    {
        IS_OK_W_EXIT(parseArgument(command, &argNumber, line, argsStart, &argEnd));

        // TODO: implementation of getArgumentsOperation(). +, -, etc as functions, like +(ax, 123)
        // IS_OK_W_EXIT(getArgumentsOperation( line, &argEnd));
    }
    command->argumentsCount = argNumber;

    return EXIT_CODES::NO_ERRORS;
}

EXIT_CODES parseArgument(command_t *command, int *argNumber, text_line_t *line, const int argsStart, int *argEnd)
{
    // Error check
    if (command == NULL || argNumber == NULL || line == NULL || argEnd == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::PASSED_OBJECT_IS_NULLPTR);
        return EXIT_CODES::PASSED_OBJECT_IS_NULLPTR;
    }

    // Parse one argument
    int newArgEnd = 0;
    char reg[2] = {};
    if (sscanf(&line->beginning[argsStart], REGISTER_FORMAT, reg, &newArgEnd) == 1)
    {
        IS_OK_W_EXIT(checkRegisterForCorrectness(reg));
        strcpy(command->arguments[*argNumber], reg);

        SET_MRI_REGISTER(command->argsMRI[*argNumber]);
    }
    else
    {
        double imm = 0;
        if (sscanf(&line->beginning[argsStart], IMMEDIATE_VALUE_FORMAT, &imm, &newArgEnd) == 1)
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

    *argEnd += newArgEnd;
    ++(*argNumber);

    return EXIT_CODES::NO_ERRORS;
}

EXIT_CODES checkRegisterForCorrectness(char *reg)
{
    // Error check
    if (reg == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::PASSED_OBJECT_IS_NULLPTR);
        return EXIT_CODES::PASSED_OBJECT_IS_NULLPTR;
    }

    // Check
    for (int regName = 0; regName < REGISTERS_TABLE_LENGTH; ++regName)
    {
        if (!strcmp(reg, REGISTERS_TABLE[regName].name))
        {
            return EXIT_CODES::NO_ERRORS;
        }
    }

    PRINT_ERROR_TRACING_MESSAGE(ASM_EXIT_CODES::UNKNOWN_COMMAND_REGISTER);
    return EXIT_CODES::BAD_OBJECT_PASSED;
}

// TODO: implementation getArgumentsMathOperation
/*
EXIT_CODES getArgumentsOperation(char *mathOP, text_line_t *line, int *argEnd)
{
    // Error check
    if (mathOP == NULL || line == NULL || argEnd == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::PASSED_OBJECT_IS_NULLPTR);
        return EXIT_CODES::PASSED_OBJECT_IS_NULLPTR;
    }

    // Get math operation
    switch (line->beginning[argEnd])
    {
        case '+':
            *mathOP = '+';
            break;
        case '-':
            *mathOP = '-';
            break;
        default:
            PRINT_ERROR_TRACING_MESSAGE(ASM_EXIT_CODES::BAD_MATH_OPERATION_IN_COMMAND_ARG);
            return EXIT_CODES::BAD_OBJECT_PASSED;
    }
    ++(*argEnd);

    return EXIT_CODES::NO_ERRORS;
}
*/
