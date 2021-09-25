#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "../include/stack.h"
#include "../include/settings.h"

bool isStackOk(stack_t *stack)
{
    return  stack != NULL && stack->data != NULL &&
            stack->size >= 0 && stack->size <= stack->capacity;
}

EXIT_CODES stackCtor(stack_t *stack, int stack_capacity)
{
    if (stack == NULL || stack->capacity != -1 || stack->size != -1)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::OLD_STACK_PASSED);
        return EXIT_CODES::OLD_STACK_PASSED;
    }

    stack->data = (stackElem_t *) calloc(stack_capacity, sizeof(stackElem_t));
    if (stack->data == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::BAD_STD_FUNC_RESULT);
        return EXIT_CODES::BAD_STD_FUNC_RESULT;
    }

    stack->capacity = stack_capacity;
    stack->size = 0;

    return EXIT_CODES::NO_ERRORS;
}

EXIT_CODES getNewReallocationCapacity(stack_t *stack, REALLOC_MODES mode, int *new_capacity)
{
    STACK_VERIFY(stack);

    switch (mode)
    {
        case REALLOC_MODES::DECREASE:
            *new_capacity = stack->capacity / stackReallocCoefficient * sizeof(stackElem_t);
            break;
        case REALLOC_MODES::INCREASE:
            *new_capacity = stack->capacity * stackReallocCoefficient * sizeof(stackElem_t);
            break;
        default:
            return EXIT_CODES::BAD_REALLOC_MODE_PASSED;
    }

    return EXIT_CODES::NO_ERRORS;
}

EXIT_CODES stackReallocation(stack_t *stack, REALLOC_MODES mode)
{
    STACK_VERIFY(stack);

    // Get new reallocation capacity
    int new_capacity = 0;
    IS_OK(getNewReallocationCapacity(stack, mode, &new_capacity));

    stackElem_t *temp = (stackElem_t *) realloc(stack->data, new_capacity);
    if (temp == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::BAD_STD_FUNC_RESULT);
        return EXIT_CODES::BAD_STD_FUNC_RESULT;
    }

    // Update structure variables
    stack->data = temp;
    if (mode == REALLOC_MODES::DECREASE)
    {
        stack->capacity = stack->capacity / stackReallocCoefficient;
    }
    else
    {
        stack->capacity = stackReallocCoefficient * stack->capacity;
    }

    return EXIT_CODES::NO_ERRORS;
}

EXIT_CODES stackPush(stack_t *stack, stackElem_t value)
{
    STACK_VERIFY(stack);

    if (stack->size == stack->capacity)
    {
        IS_OK(stackReallocation(stack, REALLOC_MODES::INCREASE));
    }

    stack->data[stack->size++] = value;

    return EXIT_CODES::NO_ERRORS;
}

EXIT_CODES stackPop(stack_t *stack, stackElem_t *popTo)
{
    STACK_VERIFY(stack);
    if (stack->size < 1)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::BAD_STACK_PASSED);
        return EXIT_CODES::BAD_STACK_PASSED;
    }

    if (stackReallocCoefficient * stack->size < stack->capacity)
    {
        IS_OK(stackReallocation(stack, REALLOC_MODES::DECREASE));
    }

    *popTo = stack->data[stack->size--];
    stack->data[stack->size + 1] = 0;  // FIXME: change 'poisonous' value

    return EXIT_CODES::NO_ERRORS;
}

#if defined STACK_DEBUG_MODE && STACK_DEBUG_MODE == 2
EXIT_CODES stackDump(stack_t *stack)
{
    if (stack == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::BAD_STACK_PASSED);
        return EXIT_CODES::BAD_STACK_PASSED;
    }

    fprintf(DEFAULT_ERROR_TRACING_STREAM, YELLOW"[DEBUG_INFO, %s %s]"RESET":\n", __DATE__, __TIME__);
    fprintf(DEFAULT_ERROR_TRACING_STREAM, "stack<%s> [0x%p] %s ", STRINGIFY(stackElem_t), stack, STRINGIFY(stack));
    fprintf(DEFAULT_ERROR_TRACING_STREAM, "from %s(%d), %s():\n", __FILE__, __LINE__, __func__);

    fprintf(DEFAULT_ERROR_TRACING_STREAM, "{\n");

    fprintf(DEFAULT_ERROR_TRACING_STREAM, "\t\tcapacity = %d (%s)\n", stack->capacity, VALUE_CODE_TO_STR(stack->capacity >= 0));
    fprintf(DEFAULT_ERROR_TRACING_STREAM, "\t\tsize = %d (%s)\n",
            stack->size, VALUE_CODE_TO_STR(stack->size <= stack->capacity && stack->size >= 0));
    fprintf(DEFAULT_ERROR_TRACING_STREAM, "\t\tdata[0x%p]\n", stack->data);

    if (stack->size > 0)
    {
        fprintf(DEFAULT_ERROR_TRACING_STREAM, "\t\t{\n");

        for (int element = 0; element < 10; ++element)
        {
            if (stack->data[element] != 0)  // FIXME: change 'poisonous' value
            {
                fprintf(DEFAULT_ERROR_TRACING_STREAM, "\t\t\t *[%d] = %d\n", element, stack->data[element]);
            }
            else
            {
                fprintf(DEFAULT_ERROR_TRACING_STREAM, "\t\t\t [%d] = %d (NOT USED)\n", element, stack->data[element]);
            }
        }

        fprintf(DEFAULT_ERROR_TRACING_STREAM, "\t\t\t...\n\t\t}\n");
    }

    fprintf(DEFAULT_ERROR_TRACING_STREAM, "}\n");

    return EXIT_CODES::NO_ERRORS;
}
#endif

EXIT_CODES stackDtor(stack_t *stack)
{
    STACK_VERIFY(stack);

    free(stack->data);
    stack->data = NULL;
    stack->capacity = -1;
    stack->size = -1;

    return EXIT_CODES::NO_ERRORS;
}