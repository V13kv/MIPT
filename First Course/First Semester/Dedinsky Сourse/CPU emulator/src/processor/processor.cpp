#include <math.h> // for fabs

#include "../../lib/stack/include/stack.h"
#include "../../lib/text/include/text.h"
#include "../../include/processor/processor.h"

const double EPS = 0.001;

#define MIN_RAM_SIZE                200      
#define DEFAULT_DOUBLE_VALUE        0
#define BAD_DOUBLE_VALUE            -663

#define MRI_IS_IMMEDIATE(byteCodeByte)  (byteCodeByte & 0b001) != 0
#define MRI_IS_REGISTER(byteCodeByte)   (byteCodeByte & 0b010) != 0
#define MRI_IS_MEMORY(byteCodeByte)     (byteCodeByte & 0b100) != 0
#define GET_TOTAL_ARGS(byteCodeByte)    (byteCodeByte & 0b11100000) >> 5
#define GET_GLOBAL_MRI(byteCodeByte)    (byteCodeByte & 0b00011100) >> 2
typedef unsigned int offset;

EXIT_CODES cpuCtor(cpu_t *CPU)
{
    // Error check
    if (CPU == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::PASSED_OBJECT_IS_NULLPTR);
        return EXIT_CODES::PASSED_OBJECT_IS_NULLPTR;
    }

    // Stak init
    IS_ERROR(stackCtor(&CPU->stack))
    {
        PRINT_ERROR_TRACING_MESSAGE(STACK_EXIT_CODES::BAD_STACK_PASSED);
        return EXIT_CODES::BAD_OBJECT_PASSED;
    }

    // RAM init
    CPU->RAM = (double *) calloc(MIN_RAM_SIZE, sizeof(double));
    CHECK_CALLOC_RESULT(CPU->RAM);

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

    // Stack destruction
    IS_ERROR(stackDtor(&CPU->stack))
    {
        PRINT_ERROR_TRACING_MESSAGE(STACK_EXIT_CODES::BAD_STACK_PASSED);
        return EXIT_CODES::BAD_OBJECT_PASSED;
    }

    // RAM destruction
    free(CPU->RAM);

    return EXIT_CODES::NO_ERRORS;
}

static EXIT_CODES _getRegisterValue(cpu_t *CPU, text_t *byteCode, double *result)
{
    // Error check
    if (CPU == NULL || byteCode == NULL || result == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::PASSED_OBJECT_IS_NULLPTR);
        return EXIT_CODES::PASSED_OBJECT_IS_NULLPTR;
    }

    // Get register value
    // TODO: change 255 to normal thing (think about const size array in `cpu_t`)
    byte regCode = (byte) byteCode->data[(int) CPU->regs[4].value];
    *result = CPU->regs[regCode].value;

    return EXIT_CODES::NO_ERRORS;
}

static double getRegisterValue(cpu_t *CPU, text_t *byteCode)
{
    // Error check
    if (CPU == NULL || byteCode == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::PASSED_OBJECT_IS_NULLPTR);
        return BAD_DOUBLE_VALUE;
    }

    // Get register value
    double result = 0;
    IS_ERROR(_getRegisterValue(CPU, byteCode, &result))
    {
        PRINT_ERROR_TRACING_MESSAGE(PROCESSOR_EXIT_CODES::FAIL_DURING_TAKING_REGISTER_VALUE);
        return BAD_DOUBLE_VALUE;
    }

    return result;
}

static EXIT_CODES _getImmediateValue(cpu_t *CPU, text_t *byteCode, double *result)
{
    // Error check
    if (CPU == NULL || byteCode == NULL || result == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::PASSED_OBJECT_IS_NULLPTR);
        return EXIT_CODES::PASSED_OBJECT_IS_NULLPTR;
    }

    // Get immediate (double) value
    // TODO: change 255 to normal thing (think about const size array in `cpu_t`)
    *result = *((double *) &byteCode->data[(int) CPU->regs[4].value]);

    return EXIT_CODES::NO_ERRORS;
}

static double getImmediateValue(cpu_t *CPU, text_t *byteCode)
{
    // Error check
    if (CPU == NULL || byteCode == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::PASSED_OBJECT_IS_NULLPTR);
        return BAD_DOUBLE_VALUE;
    }

    // Get immediate (double) value
    double result = 0;
    IS_ERROR(_getImmediateValue(CPU, byteCode, &result))
    {
        PRINT_ERROR_TRACING_MESSAGE(PROCESSOR_EXIT_CODES::FAIL_DURING_TAKING_IMMEDIATE_VALUE);
        return BAD_DOUBLE_VALUE;
    }

    return result;
}

static EXIT_CODES _cpuGetBytecodeValue(cpu_t *CPU, text_t *byteCode, double *result)
{
    // Error check
    if (CPU == NULL || byteCode == NULL || result == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::PASSED_OBJECT_IS_NULLPTR);
        return EXIT_CODES::PASSED_OBJECT_IS_NULLPTR;
    }

    // Get metainfo
    // TODO: change 255 to normal thing (think about const size array in `cpu_t`)
    *result = 0;
    size_t argc     = (size_t)  GET_TOTAL_ARGS(byteCode->data[(int) CPU->regs[4].value]);
    int globalMRI   = (int)     GET_GLOBAL_MRI(byteCode->data[(int) CPU->regs[4].value++]);
    for (size_t arg = 0; arg < argc; ++arg)
    {
        // Get bytecode double value
        // TODO: change 255 to normal thing (think about const size array in `cpu_t`)
        if (MRI_IS_REGISTER(byteCode->data[(int) CPU->regs[4].value]))
        {
            // TODO: change 255 to normal thing (think about const size array in `cpu_t`)
            ++CPU->regs[4].value;   

            *result += getRegisterValue(CPU, byteCode);

            // TODO: change 255 to normal thing (think about const size array in `cpu_t`)
            CPU->regs[4].value += sizeof(byte);
        }
        else if (MRI_IS_IMMEDIATE(byteCode->data[(int) CPU->regs[4].value]))
        {
            // TODO: change 255 to normal thing (think about const size array in `cpu_t`)
            ++CPU->regs[4].value;
            
            *result += getImmediateValue(CPU, byteCode);

            // TODO: change 255 to normal thing (think about const size array in `cpu_t`)
            CPU->regs[4].value += sizeof(double);
        }
        else
        {
            PRINT_ERROR_TRACING_MESSAGE(PROCESSOR_EXIT_CODES::UNKNOWN_INSTRUCTION_TYPE_ARG);
            return EXIT_CODES::BAD_OBJECT_PASSED;
        }
    } 

    // Check global MRI <-> Memory, Register, Immediate (actually for globalMRI using only memory)
    if (MRI_IS_MEMORY(globalMRI))
    {
        // TODO: check all RAM index etc. && maybe do separate function
        if (fabs(*result - fabs((int) *result)) < EPS)
        {
            *result = CPU->RAM[(int) *result];  
        }
        else
        {
            PRINT_ERROR_TRACING_MESSAGE(PROCESSOR_EXIT_CODES::BAD_TO_ACCESS_RAM_INDEX);
            return EXIT_CODES::BAD_OBJECT_PASSED;
        }
    }

    return EXIT_CODES::NO_ERRORS;
}

static double cpuGetBytecodeValue(cpu_t *CPU, text_t *byteCode)
{
    // Error check
    if (CPU == NULL || byteCode == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::PASSED_OBJECT_IS_NULLPTR);
        return BAD_DOUBLE_VALUE;
    }

    // Get bytecode double value (calculate expression if there is one)
    double result = DEFAULT_DOUBLE_VALUE;
    IS_ERROR(_cpuGetBytecodeValue(CPU, byteCode, &result))
    {
        // TODO: processor error flag that determines the stay of processor
        PRINT_ERROR_TRACING_MESSAGE(PROCESSOR_EXIT_CODES::ERROR_READING_BYTECODE_DOUBLE_VALUE);
        return BAD_DOUBLE_VALUE;
    }

    return result;
}

static EXIT_CODES _cpuPop(cpu_t *CPU, double *result)
{
    // Error check
    if (CPU == NULL || result == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::PASSED_OBJECT_IS_NULLPTR);
        return EXIT_CODES::PASSED_OBJECT_IS_NULLPTR;
    }

    // Pop
    IS_ERROR(stackPop(&CPU->stack, result))
    {
        PRINT_ERROR_TRACING_MESSAGE(PROCESSOR_EXIT_CODES::ERROR_POPPING_VALUE_FROM_STACK);
        return EXIT_CODES::BAD_OBJECT_PASSED;
    }

    return EXIT_CODES::NO_ERRORS;
}

static double cpuPop(cpu_t *CPU)
{
    // Error check
    if (CPU == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::PASSED_OBJECT_IS_NULLPTR);
        return BAD_DOUBLE_VALUE;
    }

    // Pop
    double result = DEFAULT_DOUBLE_VALUE;
    IS_ERROR(_cpuPop(CPU, &result))
    {
        // TODO: processor error flag that determines the stay of processor
        PRINT_ERROR_TRACING_MESSAGE(PROCESSOR_EXIT_CODES::ERROR_READING_DOUBLE_FROM_STACK);
        return BAD_DOUBLE_VALUE;
    }

    return result;
}

static EXIT_CODES cpuPush(cpu_t *CPU, double value)
{
    // Error check
    if (CPU == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::PASSED_OBJECT_IS_NULLPTR);
        return EXIT_CODES::PASSED_OBJECT_IS_NULLPTR;
    }

    // Push
    IS_ERROR(stackPush(&CPU->stack, value))
    {
        PRINT_ERROR_TRACING_MESSAGE(PROCESSOR_EXIT_CODES::ERROR_PUSHING_VALUE_TO_STACK);
        return EXIT_CODES::BAD_OBJECT_PASSED;
    }   

    return EXIT_CODES::NO_ERRORS;
}

static EXIT_CODES cpuGetBytecodeOffset(cpu_t *CPU, text_t *byteCode, offset *result)
{
    // Error check
    if (CPU == NULL || byteCode == NULL || result == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::PASSED_OBJECT_IS_NULLPTR);
        return EXIT_CODES::PASSED_OBJECT_IS_NULLPTR;
    }

    // Get offset
    *result = *((offset *) &byteCode->data[(int) CPU->regs[4].value]);

    return EXIT_CODES::NO_ERRORS;
}

static EXIT_CODES cpuJump(cpu_t *CPU, text_t *byteCode)
{
    // Error check
    if (CPU == NULL || byteCode == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::PASSED_OBJECT_IS_NULLPTR);
        return EXIT_CODES::PASSED_OBJECT_IS_NULLPTR;
    }

    // Get offset
    offset displacement = 0;
    IS_ERROR(cpuGetBytecodeOffset(CPU, byteCode, &displacement))
    {
        PRINT_ERROR_TRACING_MESSAGE(PROCESSOR_EXIT_CODES::ERROR_READING_COMMAND_OFFSET);
        return EXIT_CODES::BAD_OBJECT_PASSED;
    }

    // Jump
    // TODO: change 255 to normal thing (think about const size array in `cpu_t`)
    CPU->regs[4].value = displacement;

    return EXIT_CODES::NO_ERRORS;
}

static EXIT_CODES cpuCall(cpu_t *CPU, text_t *byteCode)
{
    // Error check
    if (CPU == NULL || byteCode == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::PASSED_OBJECT_IS_NULLPTR);
        return EXIT_CODES::PASSED_OBJECT_IS_NULLPTR;
    }

    // Push return address
    IS_ERROR(cpuPush(CPU, sizeof(byte) + sizeof(offset)))
    {
        PRINT_ERROR_TRACING_MESSAGE(PROCESSOR_EXIT_CODES::ERROR_PUSHING_RETURN_ADDRESS_TO_STACK);
        return EXIT_CODES::BAD_OBJECT_PASSED;
    }

    // Get offset
    offset displacement = 0;
    IS_ERROR(cpuGetBytecodeOffset(CPU, byteCode, &displacement))
    {
        PRINT_ERROR_TRACING_MESSAGE(PROCESSOR_EXIT_CODES::ERROR_READING_COMMAND_OFFSET);
        return EXIT_CODES::BAD_OBJECT_PASSED;
    }

    // Jump
    // TODO: change 255 to normal thing (think about const size array in `cpu_t`)
    CPU->regs[4].value = displacement;

    return EXIT_CODES::NO_ERRORS;
}

static EXIT_CODES cpuOut(cpu_t *CPU)
{
    // Error check
    if (CPU == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::PASSED_OBJECT_IS_NULLPTR);
        return EXIT_CODES::PASSED_OBJECT_IS_NULLPTR;
    }

    // Out
    printf("%lf\n", CPU->stack.data[(int) CPU->stack.size - 1]);

    return EXIT_CODES::NO_ERRORS;
}

static EXIT_CODES cpuIn(cpu_t *CPU)
{
    // Error check
    if (CPU == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::PASSED_OBJECT_IS_NULLPTR);
        return EXIT_CODES::PASSED_OBJECT_IS_NULLPTR;
    }

    // Get input
    double input = 0;
    scanf("%lf", &input);

    // Push
    IS_ERROR(cpuPush(CPU, input))
    {
        PRINT_ERROR_TRACING_MESSAGE(PROCESSOR_EXIT_CODES::ERROR_PUSHING_INPUTED_VALUE_TO_STACK);
        return EXIT_CODES::BAD_OBJECT_PASSED;
    }

    return EXIT_CODES::NO_ERRORS;
}

static void cpuExit(cpu_t *CPU, text_t *byteCode, int exitCode)
{
    // Error check
    if (CPU == NULL || byteCode == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::PASSED_OBJECT_IS_NULLPTR);
        exit(EXIT_FAILURE);
    }

    // Exit
    textDtor(byteCode);
    IS_ERROR(cpuDtor(CPU))
    {
        PRINT_ERROR_TRACING_MESSAGE(PROCESSOR_EXIT_CODES::FAIL_DURING_CPU_DTOR_IN_HALT);
        exit(EXIT_FAILURE);
    }

    exit(exitCode);
}

static EXIT_CODES cpuRet(cpu_t *CPU)
{
    // Error check
    if (CPU == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::PASSED_OBJECT_IS_NULLPTR);
        return EXIT_CODES::PASSED_OBJECT_IS_NULLPTR;
    }

    // Return
    // TODO: change 255 to normal thing (think about const size array in `cpu_t`)
    CPU->regs[4].value = (int) cpuPop(CPU);

    return EXIT_CODES::NO_ERRORS;
}

static EXIT_CODES cpuOutc(cpu_t *CPU)
{
    // Error check
    if (CPU == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::PASSED_OBJECT_IS_NULLPTR);
        return EXIT_CODES::PASSED_OBJECT_IS_NULLPTR;
    }

    // Output char
    printf("%c\n", (char) CPU->stack.data[(int) CPU->stack.size - 1]);

    return EXIT_CODES::NO_ERRORS;
}

#define OPDEF(unused, opcode, argc, code)   \
    case ((byte) opcode): printf("mnemonics now: %s\n", #unused); { code } break;

static EXIT_CODES cpuExecuteCommand(cpu_t *CPU, text_t *byteCode)
{
    // Error check
    if (CPU == NULL || byteCode == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::PASSED_OBJECT_IS_NULLPTR);
        return EXIT_CODES::PASSED_OBJECT_IS_NULLPTR;
    }

    // Execution
    double val     = DEFAULT_DOUBLE_VALUE;
    double val1    = DEFAULT_DOUBLE_VALUE;
    double val2    = DEFAULT_DOUBLE_VALUE;

    // TODO: change 255 to normal thing (think about const size array in `cpu_t`)
    byte opcode = (byte) byteCode->data[(int) CPU->regs[4].value++]; // 4 is IP register
    
    switch(opcode)
    {
        #include "../../include/opdefs.h"

        default:
            PRINT_ERROR_TRACING_MESSAGE(PROCESSOR_EXIT_CODES::UNKNOWN_OPCODE_BYTE);
            return EXIT_CODES::BAD_OBJECT_PASSED;
    } 

    return EXIT_CODES::NO_ERRORS;
}

#undef OPDEF

EXIT_CODES cpuExecuteBytecode(text_t *byteCode, cpu_t *CPU)
{
    // Error check
    if (byteCode == NULL || CPU == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::PASSED_OBJECT_IS_NULLPTR);
        return EXIT_CODES::PASSED_OBJECT_IS_NULLPTR;
    }
    
    // Execution
    // TODO: change 255 to normal thing (think about const size array in `cpu_t`)
    // maybe one time go through const size array, find IP opcode and save pointer to it in separate `cpu_t` variable
    while ((size_t) CPU->regs[4].value < byteCode->size)
    {
        // printf("IP. Code: %d. Value: %lf\n", CPU->regs[4].code, CPU->regs[4].value);
        // printf("\tWorking on 0x%x byte\n", (byte) byteCode->data[(int) CPU->regs[4].value]);
        IS_ERROR(cpuExecuteCommand(CPU, byteCode))
        {
            PRINT_ERROR_TRACING_MESSAGE(PROCESSOR_EXIT_CODES::BAD_BYTECODE_PASSED);
            return EXIT_CODES::BAD_OBJECT_PASSED;
        }
        stackDump(&CPU->stack);
    }

    return EXIT_CODES::NO_ERRORS;
}
