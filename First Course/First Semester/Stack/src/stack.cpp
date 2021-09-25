#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include "../include/stack.h"


bool stackOK(const stack_t *stack)
{
    return  stack != NULL &&
            stack->data != NULL &&
            stack->capacity >= minStackCapacity &&
            stack->size >= 0 &&
            stack->size <= stack->capacity;
}

void stackCtor(stack_t *stack)
{
    assert(stack != NULL && stack->data == NULL && stack->size == POISON && stack->capacity == POISON);

    // Fill the stack structure
    stack->data = (stackElem_t *) calloc(minStackCapacity, sizeof(stackElem_t));
    assert(stack->data != NULL && "[!] Got a NULL pointer after calloc function!\n");

    stack->capacity = minStackCapacity;
    stack->size = 0;
}

int getNewReallocationCapacity(stack_t *stack, stack_realloc_mode mode)
{
    assert(stackOK(stack));

    switch (mode)
    {
        case REALLOC_MODE_DECREASE:
            return stack->capacity / stackReallocCoefficient * sizeof(stackElem_t);
        case REALLOC_MODE_INCREASE:
            return stack->capacity * stackReallocCoefficient * sizeof(stackElem_t);
        default:
            assert(0 && "[!] You haven't passed mode parameter to _getNewReallocationCapacity function!\n");
            return -1;
    }
}

void stackReallocation(stack_t *stack, stack_realloc_mode mode)
{
    assert(stackOK(stack));

    // Reallocation
    stackElem_t *temp = (stackElem_t *) realloc(stack->data, getNewReallocationCapacity(stack, mode));
    assert(temp != NULL && "[!] Got a NULL pointer after realloc function!\n");

    // Update structure variables
    stack->data = temp;
    if (mode == REALLOC_MODE_DECREASE)
    {
        stack->capacity = stack->capacity / stackReallocCoefficient;
    }
    else
    {
        stack->capacity = stackReallocCoefficient * stack->capacity;
    }
}

void stackPush(stack_t *stack, stackElem_t value)
{
    assert(stackOK(stack));

    // Stack push
    if (stack->size == stack->capacity)
    {
        stackReallocation(stack, REALLOC_MODE_INCREASE);
    }
    stack->data[stack->size++] = value;

    assert(stackOK(stack));
}

stackElem_t stackPop(stack_t *stack)
{
    assert(stackOK(stack) && stack->size > 0);

    // Stack pop
    if (2 * stack->size < stack->capacity)
    {
        stackReallocation(stack, REALLOC_MODE_DECREASE);
    }

    stackElem_t value = stack->data[--stack->size];
    stack->data[stack->size + 1] = 0;

    return value;
}

void stackPrint(stack_t *stack)
{
    assert(stackOK(stack));

    // Print information
    printf("\nStack capacity = %d\n", stack->capacity);
    printf("Stack size = %d\n", stack->size);
    for (int elem = 0; elem < stack->size; ++elem)
    {
        printf("#%d -> %d\n", elem + 1, stack->data[elem]);
    }
}

void stackDtor(stack_t *stack)
{
    assert(stackOK(stack));

    // Free allocated memory
    free(stack->data);
    stack->capacity = -1;
    stack->size = -1;
}