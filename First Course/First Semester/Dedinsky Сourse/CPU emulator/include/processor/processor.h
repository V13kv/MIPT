#ifndef PROCESSOR_H
#define PROCESSOR_H

#define DEBUG_LEVEL 2
#include "../../lib/debug/debug.h"
#include "../../lib/stack/include/stack.h"
#include "../../lib/text/include/text.h"
#include "../regdefs.h"

#undef DEBUG_LEVEL

typedef unsigned char byte;
typedef unsigned int offset;

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
    CANT_POP_VALUE_INTO_IMMEDIATE,
    ERROR_DUMPING_PROCESSOR_STACK,
    ERROR_COUNTING_INTERNAL_EXPRESSION_VALUE,
    ERROR_DURING_JUMP,
    ERROR_DURING_TAKING_SQUARE_ROOT,
};

// TODO: RGBA struct for SFML: 4 char bytes and union (rgba, могу интерпретироват как int)

// ******TODO: do flags
struct cpu_t
{
    stack_t stack                       = {};
    double *RAM                         = NULL;
    double commonRegs[MAX_REGS_COUNT]   = {};  // Index is common register opcode, value - its value
    int IP                              = 0;
};

EXIT_CODES cpuCtor(cpu_t *CPU);
EXIT_CODES cpuDtor(cpu_t *CPU);
EXIT_CODES cpuDump(cpu_t *CPU, text_t *byteCode);

EXIT_CODES cpuExecuteBytecode(text_t *byteCode, cpu_t *CPU);


#endif  // PROCESSOR_H
