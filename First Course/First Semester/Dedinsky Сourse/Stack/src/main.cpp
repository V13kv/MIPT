#include <stdio.h>
#include <stdlib.h>

#include "../include/stack.h"


//FIXME: second bug hashSumCtor is opened to the user
// ----------------------------------SECOND BUG-----------------------------------
    // for (int i = 1; i < 11; ++i)
    // {
    //     stackPush(&stack, i);
    // }
    // long long protection_bytes = sizeof(stack.canaryLeft) + sizeof(stack.canaryRight);
    // stack.data = (stackElem_t *) calloc(stack.capacity * sizeof(stackElem_t) + protection_bytes, sizeof(stackElem_t));
    // int *canaryLeft = (int *) stack.data;
    // *canaryLeft = CANARY_VALUE;
    // int *canaryRight = (int *) (((char *) stack.data) + sizeof(stack.canaryLeft) + stack.capacity * sizeof(stackElem_t));
    // *canaryRight = CANARY_VALUE;
    // stack.data = (stackElem_t *) (((char *) stack.data) + sizeof(stack.canaryLeft));
    // hashSumCtor(&stack);
// -------------------------------------------------------------------------------

//TODO: support of any structures, elements, objects in stack
int main(void)
{
    stack_t stack = {};
    stackCtor(&stack, 8);

    #pragma region tests
        // -----------------------------------Hash sum and canary test-----------------------------------
        // stack.data = (int *) calloc(1, sizeof(stackElem_t));
        // IS_OK_W_EXIT(stackPop(&stack));

        // -----------------------------------Standard debug macroses test-----------------------------------
        // for (int i = 0; i < 2; ++i)
        // {
        //     IS_OK_W_EXIT(stackPush(&stack, 123));
        // }

        // for (int i = 0; i < 3; ++i)
        // {
        //     IS_OK_W_EXIT(stackPop(&stack));
        // }

        // -----------------------------------Canary test-----------------------------------
        // *((int*)(&stack)) = -1;
        // IS_OK_W_EXIT(stackReallocation(&stack, REALLOC_MODES::INCREASE));

        //-----------------------------------Hash Sum test----------------------------------
        // stack.hashSum = 1234;
    #pragma endregion tests

    stackDtor(&stack);

    return 0;
}
