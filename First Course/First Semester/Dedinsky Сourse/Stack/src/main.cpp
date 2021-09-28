#include <stdio.h>
#include <stdlib.h>

#include "../include/stack.h"

int main(void)
{
    stack_t stack = {};
    IS_OK_W_EXIT(stackCtor(&stack, 8));
    
    for (int i = 0; i < 1024; ++i)
    {
        IS_OK_W_EXIT(stackPush(&stack, i));
    }
    IS_OK_WO_EXIT(stackDump(&stack));
    stackReallocation(&stack, REALLOC_MODES::INCREASE);

    IS_OK_WO_EXIT(stackDtor(&stack));

    return 0;
}