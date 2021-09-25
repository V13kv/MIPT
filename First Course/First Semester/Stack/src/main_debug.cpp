#include <stdio.h>
#include "assert.h"

#include "../include/stack_debug.h"


int main()
{
    // Stack construction
    stack_t stack = {};
    stackCtor(&stack);
    
    stackPush(&stack, 666);
    stackPush(&stack, 12345);
    stackPush(&stack, 1251);
    stackPush(&stack, 5878676389);
    stackPush(&stack, 5846389);
    stackPush(&stack, 898);
    stackPush(&stack, 568);
    stackPush(&stack, 1362);
    stackPush(&stack, 92);
    stackPush(&stack, 92);
    stackPush(&stack, 92);
    stackPush(&stack, 92);

    for (int i = 0; i < 100; ++i)
    {
        stackPop(&stack);
    }


    // Stack destructor
    stackDtor(&stack);
    printf("All done!\n");

    return 0;
}