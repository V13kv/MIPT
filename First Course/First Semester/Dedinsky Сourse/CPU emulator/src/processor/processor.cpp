#include <math.h> // for fabs

#include "../../lib/colors/colors.h"
#include "../../lib/stack/include/stack.h"
#include "../../lib/text/include/text.h"

#include "../../include/processor/processor.h"
#include "../../include/processor/settings.h"

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

EXIT_CODES cpuDump(cpu_t *CPU, text_t *byteCode)
{
    // Error check
    if (CPU == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::PASSED_OBJECT_IS_NULLPTR);
        return EXIT_CODES::PASSED_OBJECT_IS_NULLPTR;
    }

    // Dump registers
    printf(GREEN "$AX" RESET ": %lf\n", CPU->commonRegs[0]);
    printf(GREEN "$BX" RESET ": %lf\n", CPU->commonRegs[1]);
    printf(GREEN "$CX" RESET ": %lf\n", CPU->commonRegs[2]);
    printf(GREEN "$DX" RESET ": %lf\n", CPU->commonRegs[3]);
    printf(RED "$IP" RESET ": %d -> 0x%x\n\n", CPU->IP, (byte) byteCode->data[CPU->IP]);

    // Dump stack
    IS_ERROR(stackDump(&CPU->stack))
    {
        PRINT_ERROR_TRACING_MESSAGE(PROCESSOR_EXIT_CODES::ERROR_DUMPING_PROCESSOR_STACK);
        return EXIT_CODES::BAD_OBJECT_PASSED;
    }
    putchar('\n');

    // Dump RAM
    for (int cell = 0; cell < RAM_CELLS_TO_DUMP; ++cell)
    {
        printf("[%lf]", CPU->RAM[cell]);
    }
    printf("\n\n");

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
    byte regCode = (byte) byteCode->data[CPU->IP];
    *result = CPU->commonRegs[regCode];

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
    *result = *((double *) &byteCode->data[CPU->IP]);

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

static EXIT_CODES __cpuCountInternalExpressionValue(cpu_t *CPU, text_t *byteCode, size_t argc, double *result)
{
    // Error check
    if (CPU == NULL || byteCode == NULL || result == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::PASSED_OBJECT_IS_NULLPTR);
        return EXIT_CODES::PASSED_OBJECT_IS_NULLPTR;
    }

    // Count expression value
    *result = 0;
    for (size_t arg = 0; arg < argc; ++arg)
    {
        // Get bytecode double value
        if (MRI_IS_REGISTER(byteCode->data[CPU->IP]))  // CPU->IP is pointing to byte after globalMRI
        {
            ++CPU->IP;   

            *result += getRegisterValue(CPU, byteCode);

            CPU->IP += sizeof(byte);
        }
        else if (MRI_IS_IMMEDIATE(byteCode->data[CPU->IP]))
        {
            ++CPU->IP;
            
            *result += getImmediateValue(CPU, byteCode);

            CPU->IP += sizeof(double);
        }
        else
        {
            PRINT_ERROR_TRACING_MESSAGE(PROCESSOR_EXIT_CODES::UNKNOWN_INSTRUCTION_TYPE_ARG);
            return EXIT_CODES::BAD_OBJECT_PASSED;
        }
    }

    return EXIT_CODES::NO_ERRORS;
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
    size_t argc     = (size_t)  GET_TOTAL_ARGS(byteCode->data[CPU->IP]);
    int globalMRI   = (int)     GET_GLOBAL_MRI(byteCode->data[CPU->IP++]);
    IS_ERROR(__cpuCountInternalExpressionValue(CPU, byteCode, argc, result))
    {
        PRINT_ERROR_TRACING_MESSAGE(PROCESSOR_EXIT_CODES::ERROR_COUNTING_INTERNAL_EXPRESSION_VALUE);
        return EXIT_CODES::BAD_OBJECT_PASSED;
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

    //TODO: EXIT_PANIC
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
    *result = *((offset *) &byteCode->data[CPU->IP]);

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
    CPU->IP = displacement;

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
    IS_ERROR(cpuPush(CPU, CPU->IP + sizeof(offset)))
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
    CPU->IP = displacement;

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
    printf("%lf", cpuPop(CPU));

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
    CPU->IP = (int) cpuPop(CPU);

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
    // printf("HERE\n");
    // Output char
    // printf("HERE: %c\n", (int) CPU->stack.data[CPU->stack.size - 1]);
    printf("%c\n", (int) CPU->stack.data[CPU->stack.size - 1]);

    return EXIT_CODES::NO_ERRORS;
}

// TODO: cpuCmp() implementation
/*
static EXIT_CODES cpuCmp(cpu_t *CPU, double val1, double val2)
{
    // Error check
    if (CPU == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::PASSED_OBJECT_IS_NULLPTR);
        return EXIT_CODES::PASSED_OBJECT_IS_NULLPTR;
    }

    // Compare
    // TODO: put comparison result as flag into processor
    if (fabs(val1 - val2) < EPS)
    {
        // Values are equal
        
    }

    return EXIT_CODES::NO_ERRORS;
}
*/

static EXIT_CODES cpuMoveValue(cpu_t *CPU, text_t *byteCode, double value)
{
    // Error check
    if (CPU == NULL || byteCode == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::PASSED_OBJECT_IS_NULLPTR);
        return EXIT_CODES::PASSED_OBJECT_IS_NULLPTR;
    }

    // Move value
    size_t argc     = (size_t)  GET_TOTAL_ARGS(byteCode->data[CPU->IP]);
    int globalMRI   = (int)     GET_GLOBAL_MRI(byteCode->data[CPU->IP++]);
    if (MRI_IS_MEMORY(globalMRI))
    {
        double result = 0;
        IS_ERROR(__cpuCountInternalExpressionValue(CPU, byteCode, argc, &result))
        {
            PRINT_ERROR_TRACING_MESSAGE(PROCESSOR_EXIT_CODES::ERROR_COUNTING_INTERNAL_EXPRESSION_VALUE);
            return EXIT_CODES::BAD_OBJECT_PASSED;
        }

        // TODO: check all RAM index etc. && maybe do separate function
        // TODO: copy-paste do function or something
        if (fabs(result - fabs((int) result)) < EPS)
        {
            CPU->RAM[(int) result] = value;
        }
        else
        {
            PRINT_ERROR_TRACING_MESSAGE(PROCESSOR_EXIT_CODES::BAD_TO_ACCESS_RAM_INDEX);
            return EXIT_CODES::BAD_OBJECT_PASSED;
        }       
    }
    else
    {
        if (MRI_IS_REGISTER(byteCode->data[CPU->IP]))
        {
            // Move value into register
            ++CPU->IP;

            byte regCode = (byte) byteCode->data[CPU->IP];
            CPU->commonRegs[regCode] = value;

            CPU->IP += sizeof(byte);
        }
        else
        {
            PRINT_ERROR_TRACING_MESSAGE(PROCESSOR_EXIT_CODES::CANT_POP_VALUE_INTO_IMMEDIATE);
            return EXIT_CODES::BAD_OBJECT_PASSED;
        }
    }

    return EXIT_CODES::NO_ERRORS;
}

//FIXME: CHECK
static EXIT_CODES cpuCmp(cpu_t *CPU)
{
    // Error check
    if (CPU == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::PASSED_OBJECT_IS_NULLPTR);
        return EXIT_CODES::PASSED_OBJECT_IS_NULLPTR;
    }  

    // Compare
    double val1 = CPU->stack.data[CPU->stack.size - 1];
    double val2 = CPU->stack.data[CPU->stack.size - 2];
    if (fabs(val1 - val2) < EPS)
    {
        // Doubles are equal
        IS_ERROR(cpuPush(CPU, DOUBLES_ARE_EQUAL))
        {
            PRINT_ERROR_TRACING_MESSAGE(PROCESSOR_EXIT_CODES::ERROR_PUSHING_VALUE_TO_STACK);
            return EXIT_CODES::BAD_OBJECT_PASSED;
        }
    }
    else if (val1 > val2)
    {
        // First is greater
        IS_ERROR(cpuPush(CPU, FIRST_DOUBLE_IS_GREATER))
        {
            PRINT_ERROR_TRACING_MESSAGE(PROCESSOR_EXIT_CODES::ERROR_PUSHING_VALUE_TO_STACK);
            return EXIT_CODES::BAD_OBJECT_PASSED;
        }
    }
    else
    {
        // First is lower
        IS_ERROR(cpuPush(CPU, FIRST_DOUBLE_IS_LOWER))
        {
            PRINT_ERROR_TRACING_MESSAGE(PROCESSOR_EXIT_CODES::ERROR_PUSHING_VALUE_TO_STACK);
            return EXIT_CODES::BAD_OBJECT_PASSED;
        }
    }

    return EXIT_CODES::NO_ERRORS;
}

//FIXME: CHECK
static EXIT_CODES cpuJE(cpu_t *CPU, text_t *byteCode)
{
    // Error check
    if (CPU == NULL || byteCode == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::PASSED_OBJECT_IS_NULLPTR);
        return EXIT_CODES::PASSED_OBJECT_IS_NULLPTR;
    }

    // Jump if equal
    int cmpResult = (int) CPU->stack.data[CPU->stack.size - 1];
    switch (cmpResult)
    {
        case 0:
            IS_ERROR(cpuJump(CPU, byteCode))
            {
                PRINT_ERROR_TRACING_MESSAGE(PROCESSOR_EXIT_CODES::ERROR_DURING_JUMP);
                return EXIT_CODES::BAD_OBJECT_PASSED;
            }
            cpuPop(CPU);
            break;

        case -1:
        case 1:
        default:
            CPU->IP += sizeof(offset);
            break;
    }

    return EXIT_CODES::NO_ERRORS;
}

//FIXME: CHECK
static EXIT_CODES cpuJL(cpu_t *CPU, text_t *byteCode)
{
    // Error check
    if (CPU == NULL || byteCode == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::PASSED_OBJECT_IS_NULLPTR);
        return EXIT_CODES::PASSED_OBJECT_IS_NULLPTR;
    }

    // Jump if equal
    int cmpResult = (int) CPU->stack.data[CPU->stack.size - 1];
    switch (cmpResult)
    {
        case -1:
            IS_ERROR(cpuJump(CPU, byteCode))
            {
                PRINT_ERROR_TRACING_MESSAGE(PROCESSOR_EXIT_CODES::ERROR_DURING_JUMP);
                return EXIT_CODES::BAD_OBJECT_PASSED;
            }
            cpuPop(CPU);
            break;

        case 0:
        case 1:
        default:
            CPU->IP += sizeof(offset);
            break;
    }

    return EXIT_CODES::NO_ERRORS;
}

// FIXME: CHECK
static EXIT_CODES cpuJG(cpu_t *CPU, text_t *byteCode)
{
    // Error check
    if (CPU == NULL || byteCode == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::PASSED_OBJECT_IS_NULLPTR);
        return EXIT_CODES::PASSED_OBJECT_IS_NULLPTR;
    }

    // Jump if equal
    int cmpResult = (int) CPU->stack.data[CPU->stack.size - 1];
    switch (cmpResult)
    {
        case 1:
            IS_ERROR(cpuJump(CPU, byteCode))
            {
                PRINT_ERROR_TRACING_MESSAGE(PROCESSOR_EXIT_CODES::ERROR_DURING_JUMP);
                return EXIT_CODES::BAD_OBJECT_PASSED;
            }
            cpuPop(CPU);
            break;

        case -1:
        case 0:
        default:
            CPU->IP += sizeof(offset);
            break;
    }

    return EXIT_CODES::NO_ERRORS;
}

#define OPDEF(unused, opcode, argc, code)   \
    case ((byte) opcode): printf("mnemonics now: %s\n", #unused);{ code } break; //

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

    byte opcode = (byte) byteCode->data[CPU->IP++];    
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
    while ((size_t) CPU->IP < byteCode->size)
    {
        // cpuDump(CPU, byteCode);
        // printf("IP. Code: %d. Value: %lf\n", CPU->regs[4].code, CPU->regs[4].value);
        // printf("\tWorking on 0x%x byte\n", (byte) byteCode->data[(int) CPU->regs[4].value]);
        IS_ERROR(cpuExecuteCommand(CPU, byteCode))
        {
            PRINT_ERROR_TRACING_MESSAGE(PROCESSOR_EXIT_CODES::BAD_BYTECODE_PASSED);
            return EXIT_CODES::BAD_OBJECT_PASSED;
        }
        // stackDump(&CPU->stack);
    }

    return EXIT_CODES::NO_ERRORS;
}
