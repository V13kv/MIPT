#include <stdio.h>
#include <stdlib.h>

#include "../include/stack.h"

int main(void)
{
    stack_t stack = {};
    IS_OK_W_EXIT(stackCtor(&stack, 8));

    for (int i = 0; i < 2047; ++i)
    {
        IS_OK_W_EXIT(stackPush(&stack, i));
    }

    for (int i = 0; i < 2046; ++i)
    {
        IS_OK_W_EXIT(stackPop(&stack));
    }
    IS_OK_W_EXIT(stackDump(&stack));
    stackReallocation(&stack, REALLOC_MODES::INCREASE);

    IS_OK_W_EXIT(stackDtor(&stack));

    return 0;
}