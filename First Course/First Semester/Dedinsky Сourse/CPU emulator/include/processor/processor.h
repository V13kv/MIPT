#ifndef PROCESSOR_H
#define PROCESSOR_H

#define DEBUG_LEVEL 1
#include "../../lib/debug/debug.h"
#include "../../lib/stack/include/stack.h"
#include "../../lib/text/include/text.h"

typedef unsigned char byte;

enum class PROCESSOR_EXIT_CODES
{
    UNKNOWN_OPCODE_BYTE,
    BAD_BYTECODE_PASSED,
    BYTES_EXECUTION_FAILURE,
    ERROR_READING_BYTECODE_DOUBLE_VALUE,
    ERROR_READING_DOUBLE_FROM_STACK,
    ERROR_PUSHING_VALUE_TO_STACK,
    ERROR_POPPING_VALUE_FROM_STACK,
    ERROR_READING_COMMAND_OFFSET,
    ERROR_PUSHING_RETURN_ADDRESS_TO_STACK,
    ERROR_PUSHING_INPUTED_VALUE_TO_STACK,
    UNKNOWN_INSTRUCTION_TYPE_ARG,
    FAIL_DURING_TAKING_REGISTER_VALUE,
    FAIL_DURING_TAKING_IMMEDIATE_VALUE,
    FAIL_DURING_CPU_DTOR_IN_HALT,
    BAD_TO_ACCESS_RAM_INDEX,
};

struct common_reg_t
{
    const int code;
    double value;
};

struct ip_reg_t
{
    const int code;
    int value;
};

// TODO: do normal regs and IP
// TODO: do flags
struct cpu_t
{
    stack_t stack   = {};
    double *RAM     = NULL;
    
    #define REGDEF(unused, regCode, ...) { .code = regCode, .value = 0 },
        
        common_reg_t regs[5] = {
            #include "../regdefs.h"
        };
        
    #undef REGDEF

    // ip_reg_t IP = {};
};

EXIT_CODES cpuCtor(cpu_t *CPU);
EXIT_CODES cpuDtor(cpu_t *CPU);

EXIT_CODES cpuExecuteBytecode(text_t *byteCode, cpu_t *CPU);


#endif  // PROCESSOR_H
