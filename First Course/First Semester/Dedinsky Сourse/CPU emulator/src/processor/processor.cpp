#include "../../include/processor/processor.h"
#include "../../lib/stack/include/stack.h"
#include "../../lib/text/include/text.h"

EXIT_CODES cpuCtor(cpu_t *CPU)
{
    // Error check
    if (CPU == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::PASSED_OBJECT_IS_NULLPTR);
        return EXIT_CODES::PASSED_OBJECT_IS_NULLPTR;
    }

    // Initialization
    IS_ERROR(stackCtor(CPU->stack))
    {
        PRINT_ERROR_TRACING_MESSAGE(STACK_EXIT_CODES::BAD_STACK_PASSED);
        return EXIT_CODES::BAD_OBJECT_PASSED;
    }

    return EXIT_CODES::NO_ERRORS;
}

EXIT_CODES cpuDtor(cpu_t *CPU)
{
    // Error check
    if (CPU == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::PASSED_OBJECT_IS_NULLPTR);
        return EXIT_CODES::PASSED_OBJECT_IS_NULLPTR;
    }

    // Destruction
    IS_ERROR(stackDtor(CPU->stack))
    {
        PRINT_ERROR_TRACING_MESSAGE(STACK_EXIT_CODES::BAD_STACK_PASSED);
        return EXIT_CODES::BAD_OBJECT_PASSED;
    }

    return EXIT_CODES::NO_ERRORS;
}

//TODO: refactor stack code to support double
//TODO: support to interact with registers values
static EXIT_CODES cpuExecuteCommand(cpu_t *CPU, byte *data)
{
    // Error check
    if (CPU == NULL || data == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::PASSED_OBJECT_IS_NULLPTR);
        return EXIT_CODES::PASSED_OBJECT_IS_NULLPTR;
    }

    // Execution
    // TODO: implementation
    // byte opcode = ;
}

EXIT_CODES cpuExecuteBytecode(text_t *byteCode, cpu_t *CPU)
{
    // Error check
    if (byteCode == NULL || CPU == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::PASSED_OBJECT_IS_NULLPTR);
        return EXIT_CODES::PASSED_OBJECT_IS_NULLPTR;
    }

    // Execution
    size_t byte = 0;
    while (byte < byteCode->size)
    {
        IS_OK_W_EXIT(cpuExecuteCommand(CPU, byteCode->data[byte]));
    }

    return EXIT_CODES::NO_ERRORS;
}
