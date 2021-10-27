#include <ctype.h>  // for isalpha && isdigit
#include <stdio.h>  // for sscanf
#include <string.h>  // for strcmp && strlen && strstr && memset

#include "../include/commands.h"
#include "../include/registers.h"

EXIT_CODES getMnemonicsOpcode(const char *const mnemonics, int *const opcode)
{
    // Error check
    if (mnemonics == NULL || opcode == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::BAD_OBJECT_PASSED);
        return EXIT_CODES::BAD_OBJECT_PASSED;
    }

    // Get command opcode
    for (int command = 0; command < (int) COMMAND_OPCODES::TOTAL_COMMANDS; ++command)
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

EXIT_CODES getCommandMRI(const command_t *const command, int *const mri)
{
    // Error check
    if (command == NULL || mri == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::PASSED_OBJECT_IS_NULLPTR);
        return EXIT_CODES::PASSED_OBJECT_IS_NULLPTR;
    }
    
    // Get arguments types
    int strLength = (int) strlen(command->arguments);
    for (int chr = 0; chr < strLength; ++chr)
    {
        if (isdigit(command->arguments[chr]))
        {
            *mri |= 0b001;
        }
        else if (isalpha(command->arguments[chr]))  // For registers
        {
            *mri |= 0b010;
        }
        else if (command->arguments[chr] == '[')  // For memory
        {
            *mri |= 0b100;
        }
    }

    return EXIT_CODES::NO_ERRORS;
}

EXIT_CODES getCommandArgImmValue(const command_t *const command, double *const imm)
{
    // Error check
    if (command == NULL || imm == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::PASSED_OBJECT_IS_NULLPTR);
        return EXIT_CODES::PASSED_OBJECT_IS_NULLPTR;
    }

    // Get command arg immediate value
    if (sscanf(command->arguments, "%*[^0-9]%lf", imm) != 1)
    {
        if (sscanf(command->arguments, "%lf", imm) != 1)
        {
            PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::BAD_STD_FUNC_RESULT);
            return EXIT_CODES::BAD_STD_FUNC_RESULT;
        }
    }

    return EXIT_CODES::NO_ERRORS;
}

EXIT_CODES getCommandArgRegisterOpcode(const command_t *const command, int *const regOpcode)
{
    // Error check
    if (command == NULL || regOpcode == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::PASSED_OBJECT_IS_NULLPTR);
        return EXIT_CODES::PASSED_OBJECT_IS_NULLPTR;
    }

    // Get command arg register
    for (int reg = 0; reg < (int) REGISTER_OPCODES::TOTAL_REGISTERS; ++reg)
    {
        if (strstr(command->arguments, registers[reg].name))
        {
            *regOpcode = registers[reg].opcode;
            return EXIT_CODES::NO_ERRORS;
        }
    }

    PRINT_ERROR_TRACING_MESSAGE(ASM_EXIT_CODE::NO_SUCH_REGISTER_FOUND);
    return EXIT_CODES::BAD_OBJECT_PASSED;
}

EXIT_CODES resetCommand(command_t * command)
{
    // Error check
    if (command == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::PASSED_OBJECT_IS_NULLPTR);
        return EXIT_CODES::PASSED_OBJECT_IS_NULLPTR;
    }

    // Reset
    memset(command->mnemonics, 0, sizeof(command->mnemonics));
    memset(command->arguments, 0, sizeof(command->arguments));
    command->opcode             = -1;
    command->MRI                = 0;
    command->arguments_count    = -1;
    command->bytes              = 0;

    return EXIT_CODES::NO_ERRORS;
}
