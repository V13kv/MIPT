#ifndef PROCESSOR_H
#define PROCESSOR_H

#include <stdbool.h>

#define DEBUG_LEVEL 1
#include "../../lib/debug/debug.h"

#include "../../lib/text/include/text.h"
// #include "../../lib/stack/include/stack.h"
#include "../registers.h"


struct processor_t
{
    register_t *const regs = registers;
};

// EXIT_CODES cpuCtor(processor_t *cpu);
// EXIT_CODES cpuDtor(processor_t *cpu);

EXIT_CODES execute(text_t *byteCode, processor_t *cpu);

#endif  // PROCESSOR_H
