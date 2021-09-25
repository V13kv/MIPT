#ifndef STACK_DEBUG_H
#define STACK_DEBUG_H

#include <stdbool.h>
#include <stdio.h>

#include "../include/settings.h"

typedef enum
{
    REALLOC_MODE_INCREASE,
    REALLOC_MODE_DECREASE
} stack_realloc_mode;

struct stack_t
{
    stackElem_t *data = NULL;
    int capacity = -1;
    int size = -1;
};

struct StackDump
{
    const char *file;
    const int line;
    const char *func;
    const char *type_of_func;
    const char *variable_name;
    const char *variable_type;
};

void stackCtor(stack_t *stack);
void stackDtor(stack_t *stack);

void stackDump(const stack_t *stack, StackDump *dump, FILE *out);

int getNewReallocationCapacity(stack_t *stack, stack_realloc_mode mode);
void stackReallocation(stack_t *stack, stack_realloc_mode mode);

void stackPush(stack_t *stack, stackElem_t value);
stackElem_t stackPop(stack_t *stack);


#endif  // STACK_DEBUG_H