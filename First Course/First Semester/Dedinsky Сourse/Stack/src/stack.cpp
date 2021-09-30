#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "../include/stack.h"

//FIXME: do normal error tracing (use reasonable macroses)
#if defined(STACK_CANARY) && STACK_CANARY == 1  
    
    EXIT_CODES canaryCheck(stack_t *stack, bool *result)
    {
        //FIXME: 14-24 lines are copy-paste think how to implement better
        // Error check
        if (stack == NULL)
        {
            PRINT_ERROR_TRACING_MESSAGE(STACK_EXIT_CODES::PASSED_STACK_IS_NULLPTR);
            return EXIT_CODES::BAD_OBJECT_PASSED;
        }

        if (stack->data == NULL || stack->size < 0 || stack->size > stack->capacity)
        {
            PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::BAD_OBJECT_PASSED);
            return EXIT_CODES::BAD_OBJECT_PASSED;
        }

        // Stack structure canary check
        if (stack->canaryLeft != CANARY_VALUE || stack->canaryRight != CANARY_VALUE)
        {
            *result = false;

            PRINT_ERROR_TRACING_MESSAGE(STACK_EXIT_CODES::STACK_STRUCTURE_CANARY_IS_DAMAGED);
            return EXIT_CODES::BAD_OBJECT_PASSED;
        }

        // Stack data canary check
        // FIXME: stack->data pointer can be changed to different location!
        if ( *(stack->data - 1) != CANARY_VALUE || stack->data[stack->capacity] != CANARY_VALUE)
        {
            *result = false;

            PRINT_ERROR_TRACING_MESSAGE(STACK_EXIT_CODES::STACK_DATA_CANARY_IS_DAMAGED);
            return EXIT_CODES::BAD_OBJECT_PASSED;
        }

        return EXIT_CODES::NO_ERRORS;
    }

    EXIT_CODES canaryCtor(stack_t *stack, int stack_capacity)
    {
        // Error check
        if (stack == NULL)
        {
            PRINT_ERROR_TRACING_MESSAGE(STACK_EXIT_CODES::PASSED_STACK_IS_NULLPTR);
            return EXIT_CODES::BAD_OBJECT_PASSED;
        }

        if (stack->capacity != -1 || stack->size != -1)
        {
            PRINT_ERROR_TRACING_MESSAGE(STACK_EXIT_CODES::OLD_STACK_PASSED);
            return EXIT_CODES::BAD_OBJECT_PASSED;
        }

        // Canary construction
        stack->data[0] = CANARY_VALUE;
        stack->data[stack_capacity + 1] = CANARY_VALUE;
        stack->data++;

        return EXIT_CODES::NO_ERRORS;
    }
    
#endif

//TODO: think of implementing comparison between saved hashSum and hashSum after lib function is done
#if defined(STACK_HASH) && STACK_HASH == 1  

    EXIT_CODES hashCheck(stack_t *stack)
    {
        // Error check
        OBJECT_VERIFY(stack);

        // HashSum check
        //FIXME: hashSum can be easily changed
        if (stack->hashSum == )
        {
            
        }


        return EXIT_CODES::NO_ERRORS;
    }

    /*
    EXIT_CODES stackDataHashSum(stack_t *stack, long long int *tempHashSum)
    {
        // Error check
        OBJECT_VERIFY(stack);

        // Calculation
        *tempHashSum += (&stack->data[stack->size] - &stack->data[0]) * sizeof(stackElem_t);
    }
    */

    EXIT_CODES hashSumCtor(stack_t *stack)
    {
        // Error check
        OBJECT_VERIFY(stack);

        // Calculation
        stack->hashSum += sizeof(stack->data) + sizeof(stack->capacity) + sizeof(stack->size)

        #if defined(STACK_CANARY) && STACK_CANARY == 1
            stack->hashSum += sizeof(stack->canaryLeft) + sizeof(stack->canaryRight);
        #endif

        return EXIT_CODES::NO_ERRORS;
    }

#endif

bool stackOk(stack_t *stack)
{
    bool stack_check =  stack != NULL && stack->data != NULL &&
                        stack->size >= 0 && stack->size <= stack->capacity;
    
    #if defined(STACK_CANARY) && STACK_CANARY == 1  
        bool canary_is_ok = true;
        canaryCheck(stack, &canary_is_ok);

        stack_check = stack_check && canary_is_ok;
    #endif 

    #if defined(STACK_HASH) && STACK_HASH == 1  
        stack_check = stack_check && hashCheck(stack);
    #endif

    return stack_check;
}

EXIT_CODES sprayPoison(stack_t *stack)
{
    // Error check
    OBJECT_VERIFY(stack);

    for (int i = stack->size; i < stack->capacity; ++i)
    {
        stack->data[i] = POISON;
    }

    return EXIT_CODES::NO_ERRORS;
}

//Doing: hash sum
EXIT_CODES stackCtor(stack_t *stack, int stack_capacity)
{
    // Error check
    if (stack == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(STACK_EXIT_CODES::PASSED_STACK_IS_NULLPTR);
        return EXIT_CODES::BAD_OBJECT_PASSED;
    }

    if (stack->capacity != -1 || stack->size != -1)
    {
        PRINT_ERROR_TRACING_MESSAGE(STACK_EXIT_CODES::OLD_STACK_PASSED);
        return EXIT_CODES::BAD_OBJECT_PASSED;
    }

    // Protection bytes for hashSum field
    int protection_capacity_increse = 0;
    #if defined(STACK_HASH) && STACK_HASH == 1
        protection_capacity_increse += sizeof(stack->hashSum);
    #endif
    
    // Protection bytes for canary fields
    #if defined(STACK_CANARY) && STACK_CANARY == 1
        protection_capacity_increse += (LEFT_CANARY_SIZE + RIGHT_CANARY_SIZE);
    #endif

    // Memory allocation (for stack elements)
    stack->data = (stackElem_t *) malloc(stack_capacity * sizeof(stackElem_t) + protection_capacity_increse);
    if (stack->data == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::BAD_STD_FUNC_RESULT);
        return EXIT_CODES::BAD_STD_FUNC_RESULT;
    }

    // Construct canary fields
    #if defined(STACK_CANARY) && STACK_CANARY == 1
        IS_OK_W_EXIT(canaryCtor(stack, stack_capacity));
    #endif

    // Construct hasSum field
    #if defined(STACK_HASH) && STACK_HASH == 1
        IS_OK_W_EXIT(hashSumCtor(stack));
    #endif

    stack->capacity = stack_capacity;
    stack->size = 0;

    // Poison new fields
    IS_OK_W_EXIT(sprayPoison(stack));

    return EXIT_CODES::NO_ERRORS;
}

EXIT_CODES getNewReallocationCapacity(stack_t *stack, REALLOC_MODES mode, int *new_capacity)
{
    // Error check
    OBJECT_VERIFY(stack);

    switch (mode)
    {
        case REALLOC_MODES::DECREASE:
            *new_capacity = stack->capacity / stackReallocCoefficient;
            break;
        case REALLOC_MODES::INCREASE:
            *new_capacity = stack->capacity * stackReallocCoefficient;
            break;
        default:
            return EXIT_CODES::BAD_OBJECT_PASSED;
    }

    return EXIT_CODES::NO_ERRORS;
}

EXIT_CODES stackReallocation(stack_t *stack, REALLOC_MODES mode)
{
    // Error check
    OBJECT_VERIFY(stack);

    // Get new reallocation capacity
    int new_capacity = 0;
    IS_OK_W_EXIT(getNewReallocationCapacity(stack, mode, &new_capacity));

    #if defined(STACK_CANARY) && STACK_CANARY == 1
        stackElem_t *temp = (stackElem_t *) realloc(stack->data - 1,  // because we want to free canaries (left and right)
                                                    LEFT_CANARY_SIZE + new_capacity * sizeof(stackElem_t) + RIGHT_CANARY_SIZE);
    #else
        stackElem_t *temp = (stackElem_t *) realloc(stack->data, new_capacity  * sizeof(stackElem_t));
    #endif
    
    if (temp == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::BAD_STD_FUNC_RESULT);
        return EXIT_CODES::BAD_STD_FUNC_RESULT;
    }

    #if defined(STACK_CANARY) && STACK_CANARY == 1
        // Set canaries
        temp[0] = CANARY_VALUE;
        temp[new_capacity + 1] = CANARY_VALUE;
        temp++;
    #endif

    // Update structure variables
    stack->data = temp;
    stack->capacity = new_capacity;

    // Poison new fields
    IS_OK_W_EXIT(sprayPoison(stack));

    return EXIT_CODES::NO_ERRORS;
}

EXIT_CODES stackPush(stack_t *stack, stackElem_t value)
{
    // Error check
    OBJECT_VERIFY(stack);

    if (stack->size == stack->capacity)
    {
        IS_OK_W_EXIT(stackReallocation(stack, REALLOC_MODES::INCREASE));
    }

    stack->data[stack->size++] = value;

    return EXIT_CODES::NO_ERRORS;
}

EXIT_CODES stackPop(stack_t *stack, stackElem_t popTo)
{
    // Error check
    OBJECT_VERIFY(stack);

    if (stack->size < 1)
    {
        PRINT_ERROR_TRACING_MESSAGE(STACK_EXIT_CODES::BAD_STACK_SIZE);
        return EXIT_CODES::BAD_OBJECT_PASSED;
    }

    if (stackReallocCoefficient * stack->size < stack->capacity)
    {
        IS_OK_W_EXIT(stackReallocation(stack, REALLOC_MODES::DECREASE));
    }

    popTo = stack->data[stack->size--];
    stack->data[stack->size + 1] = POISON;

    return EXIT_CODES::NO_ERRORS;
}

#if defined(STACK_DEBUG_LEVEL) && STACK_DEBUG_LEVEL == 2
    EXIT_CODES stackDump(stack_t *stack)
    {
        if (stack == NULL)
        {
            PRINT_ERROR_TRACING_MESSAGE(STACK_EXIT_CODES::PASSED_STACK_IS_NULLPTR);
            return EXIT_CODES::BAD_OBJECT_PASSED;
        }

        fprintf(DEFAULT_ERROR_TRACING_STREAM, YELLOW"[DEBUG_INFO, %s %s]"RESET":\n", __DATE__, __TIME__);
        fprintf(DEFAULT_ERROR_TRACING_STREAM, "stack<%s> [0x%p] %s ", STRINGIFY(stackElem_t), stack, STRINGIFY(stack));
        fprintf(DEFAULT_ERROR_TRACING_STREAM, "from %s(%d), %s():\n", __FILE__, __LINE__, __func__);

        fprintf(DEFAULT_ERROR_TRACING_STREAM, "{\n");

        #if defined(STACK_CANARY) && STACK_CANARY == 1
            fprintf(DEFAULT_ERROR_TRACING_STREAM, "\t\tCANARY = %d\n", stack->canaryLeft);
        #endif

        fprintf(DEFAULT_ERROR_TRACING_STREAM, "\t\tcapacity = %d (%s)\n", stack->capacity, VALUE_CODE_TO_STR(stack->capacity >= 0));
        fprintf(DEFAULT_ERROR_TRACING_STREAM, "\t\tsize = %d (%s)\n",
                stack->size, VALUE_CODE_TO_STR(stack->size <= stack->capacity && stack->size >= 0));
        fprintf(DEFAULT_ERROR_TRACING_STREAM, "\t\tdata[0x%p]\n", stack->data);

        if (stack->size > 0)
        {
            fprintf(DEFAULT_ERROR_TRACING_STREAM, "\t\t{\n");

            #if defined(STACK_CANARY) && STACK_CANARY == 1
                fprintf(DEFAULT_ERROR_TRACING_STREAM, "\t\t\t [-1] = %d (CANARY)\n", *(stack->data - 1));
            #endif

            for (int element = 0; element < 10; ++element)
            {
                if (stack->data[element] != POISON)
                {
                    fprintf(DEFAULT_ERROR_TRACING_STREAM, "\t\t\t *[%d] = %d\n", element, stack->data[element]);
                }
                else
                {
                    fprintf(DEFAULT_ERROR_TRACING_STREAM, "\t\t\t [%d] = %d (NOT USED)\n", element, stack->data[element]);
                }
            }

            fprintf(DEFAULT_ERROR_TRACING_STREAM, "\t\t\t ...\n");

            #if defined(STACK_CANARY) && STACK_CANARY == 1
                fprintf(DEFAULT_ERROR_TRACING_STREAM, "\t\t\t [%d] = %d (CANARY)\n", stack->capacity, stack->data[stack->capacity]);
            #endif

            fprintf(DEFAULT_ERROR_TRACING_STREAM, "\t\t}\n");
        }

        #if defined(STACK_CANARY) && STACK_CANARY == 1
            fprintf(DEFAULT_ERROR_TRACING_STREAM, "\t\tCANARY = %d \n", stack->canaryRight);
        #endif

        fprintf(DEFAULT_ERROR_TRACING_STREAM, "}\n");

        return EXIT_CODES::NO_ERRORS;
    }
#endif

EXIT_CODES stackDtor(stack_t *stack)
{
    OBJECT_VERIFY(stack);

    #if defined(STACK_CANARY) && STACK_CANARY == 1
        free(stack->data - 1);
    #else
        free(stack->data);
    #endif
    stack->data = NULL;
    stack->capacity = -1;
    stack->size = -1;

    return EXIT_CODES::NO_ERRORS;
}