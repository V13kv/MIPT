#include <stdbool.h>

#include "../../lib/text/include/text.h"
#include "../../lib/stack/include/stack.h"
#include "../../include/processor/processor.h"


EXIT_CODES cpuCtor(processor_t *cpu)
{
    // Error check
    if (cpu == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::PASSED_OBJECT_IS_NULLPTR);
        return EXIT_CODES::PASSED_OBJECT_IS_NULLPTR;
    }

    // Initialization
    stackCtor(cpu->stack);

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

    stackDtor(cpu->stack);

    return EXIT_CODES::NO_ERRORS;
}

EXIT_CODES execute(text_t *byteCode, processor_t *cpu)
{
    // Error check
    if (byteCode == NULL || cpu == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::PASSED_OBJECT_IS_NULLPTR);
        return EXIT_CODES::PASSED_OBJECT_IS_NULLPTR;
    }
    
    // Execution
    // TODO: take some parts from disassembler
}
