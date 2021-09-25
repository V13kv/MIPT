#include <stdio.h>
#include "assert.h"

#include "../include/stack.h"


int main()
{
    // Stack construction
    stack_t stack = {};
    stackCtor(&stack);

    stackPrint(&stack);
    for (int i = 0; i < 9; ++i)
    {
        stackPush(&stack, i*2 + 1);
    }
    stackPrint(&stack);

    for (int i = 8; i > 5; --i)
    {
        stackPop(&stack);
    }
    stackPrint(&stack);

    for (int i = 0; i < 100; ++i)
    {
        stackPop(&stack);
    }

    // Stack destructor
    stackDtor(&stack);

    return 0;
}