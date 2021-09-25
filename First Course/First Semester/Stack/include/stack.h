#ifndef STACK_H
#define STACK_H


#include <stdbool.h>

#include "../include/settings.h"

typedef enum
{
    REALLOC_MODE_INCREASE,
    REALLOC_MODE_DECREASE
} stack_realloc_mode;

struct stack_t
{
    stackElem_t *data = NULL;
    int capacity = POISON;
    int size = POISON;
};

void stackCtor(stack_t *stack);
void stackDtor(stack_t *stack);

int getNewReallocationCapacity(stack_t *stack, stack_realloc_mode mode);
void stackReallocation(stack_t *stack, stack_realloc_mode mode);

bool stackOK(const stack_t *stack);
void stackPush(stack_t *stack, stackElem_t value);
stackElem_t stackPop(stack_t *stack);
void stackPrint(stack_t *stack);

#endif  // STACK_H