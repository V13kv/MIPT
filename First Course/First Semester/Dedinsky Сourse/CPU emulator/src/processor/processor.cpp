#include <stdbool.h>

#include "../../include/mydisasm/disassembler.h"
#include "../../lib/text/include/text.h"
#include "../../lib/stack/include/stack.h"
#include "../../include/processor/processor.h"
#include "../commands.h"

/*
EXIT_CODES cpuCtor(processor_t *cpu)
{
    // Error check
    if (cpu == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::PASSED_OBJECT_IS_NULLPTR);
        return EXIT_CODES::PASSED_OBJECT_IS_NULLPTR;
    }

    // Initialization
    //stackCtor(cpu->stack);

    return EXIT_CODES::NO_ERRORS;
}

EXIT_CODES cpuDtor(processor_t *cpu)
{
    // Error check
    if (cpu == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::PASSED_OBJECT_IS_NULLPTR);
        return EXIT_CODES::PASSED_OBJECT_IS_NULLPTR;
    }

    // stackDtor(cpu->stack);

    return EXIT_CODES::NO_ERRORS;
}
*/

EXIT_CODES executeCommand(command_t *command, processor_t *cpu, stack_t *stack)
{
    // Error check
    if (command == NULL || cpu == NULL || stack == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::PASSED_OBJECT_IS_NULLPTR);
        return EXIT_CODES::PASSED_OBJECT_IS_NULLPTR;
    }

    // Execution
    
}

EXIT_CODES execute(text_t *byteCode, processor_t *cpu, stack_t *stack)
{
    // Error check
    if (byteCode == NULL || cpu == NULL || stack == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::PASSED_OBJECT_IS_NULLPTR);
        return EXIT_CODES::PASSED_OBJECT_IS_NULLPTR;
    }
    
    // Execution
    command_t command = {};
    for (size_t byte = 0; byte < byteCode->size; byteCode += command.bytes)
    {
        IS_OK_W_EXIT(resetCommand(&command));

        IS_OK_W_EXIT(decodeCommand(&byteCode->data[byte], &command));
        IS_OK_W_EXIT(executeCommand(&command, &cpu, &stack));
    }

    return EXIT_CODES::NO_ERRORS;
}
