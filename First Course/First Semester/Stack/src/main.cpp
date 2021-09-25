#include <stdio.h>
#include <stdlib.h>

#include "../include/stack.h"


int main(void)
{
    stack_t stack = {};
    IS_OK(stackCtor(&stack, 8));

    for (int i = 0; i < 8; ++i)
    {
        IS_OK(stackPush(&stack, i));
    }

    stackDump(&stack);
    stackReallocation(NULL, REALLOC_MODES::INCREASE);

    IS_OK(stackDtor(&stack));


    return 0;
}