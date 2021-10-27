#ifndef RAM_H
#define RAM_H

#define DEBUG_LEVEL 1
#include "../../lib/debug/debug.h"

struct ram_t
{
    char *data = NULL;
};

EXIT_CODES ramCtor(ram_t *ram);



#endif  // RAM_H
