#include <assert.h>
#include <stdlib.h>

#include "../include/stack_debug.h"


#define IS_OKAY(expr) (expr ? "OK" : "ERROR")

void stackCtor(stack_t *stack)
{
    if (!(stack != NULL && stack->data == NULL && stack->size == -1 && stack->capacity == -1))
    {
        StackDump dump = {__FILE__, __LINE__, __func__, "void", "stack", "int"};
        stackDump(stack, &dump, stderr);
    }

    stack->data = (stackElem_t *) calloc(minStackCapacity, sizeof(stackElem_t));
    if (stack->data == NULL)
    {
        StackDump dump = {__FILE__, __LINE__, __func__, "void", "stack", "int"};
        stackDump(stack, &dump, stderr);
    }

    stack->capacity = minStackCapacity;
    stack->size = 0;
}

int getNewReallocationCapacity(stack_t *stack, stack_realloc_mode mode)
{
    if (!(stack != NULL && stack->data != NULL && stack->size > 0 && stack->size <= stack->capacity))
    {
        StackDump dump = {__FILE__, __LINE__, __func__, "int", "stack", "int"};
        stackDump(stack, &dump, stderr);
    }

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
    if (!(stack != NULL && stack->data != NULL && stack->size > 0 && stack->size <= stack->capacity))
    {
        StackDump dump = {__FILE__, __LINE__, __func__, "void", "stack", "int"};
        stackDump(stack, &dump, stderr);
    }

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
    if (!(stack != NULL && stack->data != NULL && stack->size >= 0 && stack->size <= stack->capacity))
    {
        StackDump dump = {__FILE__, __LINE__, __func__, "void", "stack", "int"};
        stackDump(stack, &dump, stderr);
    }

    if (stack->size == stack->capacity)
    {
        stackReallocation(stack, REALLOC_MODE_INCREASE);
    }

    stack->data[stack->size++] = value;

    if (!(stack != NULL && stack->data != NULL &&  stack->size > 0 && stack->size <= stack->capacity && stack->data[stack->size - 1] == value))
    {
        StackDump dump = {__FILE__, __LINE__, __func__, "void", "stack", "int"};
        stackDump(stack, &dump, stderr);
    }
}

stackElem_t stackPop(stack_t *stack)
{
    if (!(stack != NULL && stack->data != NULL && stack->size >= 1 && stack->size < stack->capacity))
    {
        StackDump dump = {__FILE__, __LINE__, __func__, "stackElem_t", "stack", "int"};
        stackDump(stack, &dump, stderr);
    }

    if (stackReallocCoefficient * stack->size < stack->capacity)
    {
        stackReallocation(stack, REALLOC_MODE_DECREASE);
    }

    stackElem_t value = stack->data[stack->size--];
    stack->data[stack->size + 1] = 0;

    if (!(stack != NULL && stack->data != NULL && stack->size + 1 == 0 && stack->size >= 0 && stack->size < stack->capacity))
    {
        StackDump dump = {__FILE__, __LINE__, __func__, "stackElem_t", "stack", "int"};
        stackDump(stack, &dump, stderr);
    }

    return value;
}

void stackDump(const stack_t *stack, StackDump *dump, FILE *out)
{
    fprintf(out, "%s, %s:\n", __DATE__, __TIME__);
    fprintf(out, "stack<%s> [0x%p] \"%s\" ", dump->variable_type, stack, dump->variable_name);
    fprintf(out, "from %s(%d), %s %s():\n", dump->file, dump->line, dump->type_of_func, dump->func);

    fprintf(out, "{\n");
    fprintf(out, "\t\tcapacity = %d (%s)\n", stack->capacity, IS_OKAY(stack->capacity >= 0));
    fprintf(out, "\t\tsize = %d (%s)\n", stack->size, IS_OKAY(stack->size <= stack->capacity && stack->size >= 0));
    fprintf(out, "\t\tdata[0x%p]\n", stack->data);

    if (stack->size > 0)
    {
        fprintf(out, "\t\t{\n");

        for (int i = 0; i < stack->size % 10; ++i)
        {
            if (stack->data[i] != 0)
            {
                fprintf(out, "\t\t\t *[%d] = %d\n", i, stack->data[i]);
            }
            else
            {
                fprintf(out, "\t\t\t [%d] = %d (NOT USED)\n", i, stack->data[i]);
            }
        }

        fprintf(out, "\t\t}\n");
    }
    fprintf(out, "}\n");
  
    assert(0 && "Verify failed!");
}

void stackDtor(stack_t *stack)
{
    if (!(stack != NULL && stack->data != NULL && stack->size != -1 && stack->capacity != -1 && stack->size <= stack->capacity))
    {
        StackDump dump = {__FILE__, __LINE__, __func__, "void", "stack", "int"};
        stackDump(stack, &dump, stderr);
    }

    free(stack->data);
    stack->capacity = -1;
    stack->size = -1;
}