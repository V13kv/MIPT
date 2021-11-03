#ifndef PROCESSOR_H
#define PROCESSOR_H

#define DEBUG_LEVEL 1
#include "../../lib/debug/debug.h"
#include "../../lib/stack/include/stack.h"
#include "../../lib/text/include/text.h"

typedef unsigned char byte;

struct cpu_t
{
    #define REGDEF(unused, regOpcode, ...) regOpcode,
        
        const int REGISTERS_OPCODE_TABLE[] = {
            #include "../regdefs.h"
        };
        
    #undef REGDEF

    stack_t stack = {};
};

EXIT_CODES cpuCtor(cpu_t *CPU);
EXIT_CODES cpuDtor(cpu_t *CPU);

EXIT_CODES cpuExecuteBytecode(text_t *byteCode, cpu_t *CPU);


#endif  // PROCESSOR_H
