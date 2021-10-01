#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "../include/stack.h"

#if defined(STACK_CANARY) && STACK_CANARY == 1  
    
    EXIT_CODES canaryCheck(stack_t *stack, bool *result)
    {
        // Error check
        if (!stackBasicCheck(stack) || result == NULL)
        {
            *result = false;

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
        // FIXME: bug stack->data pointer can be changed to different location! (can be fixed using hashSum)
        if ( *(stack->data - 1) != CANARY_VALUE || stack->data[stack->capacity] != CANARY_VALUE)
        {
            *result = false;

            PRINT_ERROR_TRACING_MESSAGE(STACK_EXIT_CODES::STACK_DATA_CANARY_IS_DAMAGED);
            return EXIT_CODES::BAD_OBJECT_PASSED;
        }

        *result = true;
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

#if defined(STACK_HASH) && STACK_HASH == 1  

    EXIT_CODES stackHashSum(stack_t *stack, long long int *hash_sum)
    {
        // Error check
        if (!stackBasicCheck(stack) || hash_sum == NULL)
        {
            PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::BAD_OBJECT_PASSED);
            return EXIT_CODES::BAD_OBJECT_PASSED;
        }

        // Calculate structure hash sum
        (*hash_sum) = stack->capacity + stack->size + *stack->data;  // FIXME: bug *stack->data can cause undefined behaviour
        #if defined(STACK_CANARY) && STACK_CANARY == 1
            (*hash_sum) += 2 * CANARY_VALUE;
        #endif

        return EXIT_CODES::NO_ERRORS;
    }

    EXIT_CODES stackDataHashSum(stack_t *stack, long long int *hash_sum)
    {
        // Error check
        if (!stackBasicCheck(stack) || hash_sum == NULL)
        {
            PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::BAD_OBJECT_PASSED);
            return EXIT_CODES::BAD_OBJECT_PASSED;
        }

        // Calculate hash sum
        #if defined(STACK_CANARY) && STACK_CANARY == 1
            (*hash_sum) = 2 * CANARY_VALUE;  // stack->data canaries
        #endif

        //TODO: изменить сумму под универсальную вещь (сделать факторизацию, разбиение на 8,4,2,1 байты)
        //TODO: написать однобайтовую сумму char *new_data = stack->data        
        for (int i = 0; i < stack->capacity; ++i)  // all stack->data elements
        {
            // (*hash_sum) += new_data[i]
            (*hash_sum) += stack->data[i];
        }

        return EXIT_CODES::NO_ERRORS;
    }

    EXIT_CODES calculateStackHashSum(stack_t *stack, long long int *hash_sum)
    {
        // Error check
        if (!stackBasicCheck(stack) || hash_sum == NULL)
        {
            PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::BAD_OBJECT_PASSED);
            return EXIT_CODES::BAD_OBJECT_PASSED;
        }

        // Calculate full hash sum
        long long int struct_hash_sum = 0;
        IS_OK_WO_EXIT(stackHashSum(stack, &struct_hash_sum));

        long long int struct_data_hash_sum = 0;
        IS_OK_WO_EXIT(stackDataHashSum(stack, &struct_data_hash_sum));

        (*hash_sum) = struct_hash_sum + struct_data_hash_sum;

        return EXIT_CODES::NO_ERRORS;
    }

    //FIXME: hashSum can be easily changed
    EXIT_CODES hashCheck(stack_t *stack, bool *result)
    {
        // Error check
        if (!stackBasicCheck(stack) || result == NULL)
        {
            *result = false;

            PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::BAD_OBJECT_PASSED);
            return EXIT_CODES::BAD_OBJECT_PASSED;
        }

        // Hash sum check
        long long int currentHashSum = 0;
        IS_OK_WO_EXIT(calculateStackHashSum(stack, &currentHashSum));

        if (stack->hashSum != currentHashSum)
        {
            *result = false;

            PRINT_ERROR_TRACING_MESSAGE(STACK_EXIT_CODES::STACK_HASH_SUM_IS_DAMAGED);
            return EXIT_CODES::BAD_OBJECT_PASSED;
        }

        *result = true;
        return EXIT_CODES::NO_ERRORS;
    }

    EXIT_CODES hashSumCtor(stack_t *stack)
    {
        // Error check
        if (!stackBasicCheck(stack))
        {
            PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::BAD_OBJECT_PASSED);
            return EXIT_CODES::BAD_OBJECT_PASSED;
        }

        // Calculation of hash sum
        long long int currentHashSum = 0;
        IS_OK_WO_EXIT(calculateStackHashSum(stack, &currentHashSum));
        
        stack->hashSum = currentHashSum;

        return EXIT_CODES::NO_ERRORS;
    }

#endif

bool stackBasicCheck(stack_t *stack)
{
    return  stack != NULL && stack->data != NULL &&
            stack->size >= 0 && stack->size <= stack->capacity;
}

bool stackOk(stack_t *stack)
{
    bool result = stackBasicCheck(stack);
    #if defined(STACK_CANARY) && STACK_CANARY == 1  
        bool canary_is_ok = true;
        IS_OK_WO_EXIT(canaryCheck(stack, &canary_is_ok));

        result = result && canary_is_ok;
    #endif 

    #if defined(STACK_HASH) && STACK_HASH == 1  
        bool hash_is_ok = true;
        IS_OK_WO_EXIT(hashCheck(stack, &hash_is_ok));

        result = result && hash_is_ok;
    #endif

    return result;
}

EXIT_CODES sprayPoisonOnData(stack_t *stack)
{
    // Error check
    if (!stackBasicCheck(stack))
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::BAD_OBJECT_PASSED);
        return EXIT_CODES::BAD_OBJECT_PASSED;
    }

    for (int i = stack->size; i < stack->capacity; ++i)
    {
        stack->data[i] = POISON;
    }

    return EXIT_CODES::NO_ERRORS;
}

#if STACK_CANARY == 1 || STACK_HASH == 1

    EXIT_CODES stackCapacitySecurityIncrease(stack_t *stack, int *add_bytes)
    {
        // Error check
        if (stack == NULL || add_bytes == NULL)
        {
            PRINT_ERROR_TRACING_MESSAGE(STACK_EXIT_CODES::PASSED_STACK_IS_NULLPTR);
            return EXIT_CODES::BAD_OBJECT_PASSED;
        }

        // Protection bytes for hashSum field
        #if defined(STACK_HASH) && STACK_HASH == 1
            (*add_bytes) += sizeof(stack->hashSum);
        #endif
        
        // Protection bytes for canary fields
        #if defined(STACK_CANARY) && STACK_CANARY == 1
            (*add_bytes) += (LEFT_CANARY_SIZE + RIGHT_CANARY_SIZE);
        #endif

        return EXIT_CODES::NO_ERRORS;
    }

#endif

// TODO: change struct default values to macroses
EXIT_CODES stackCtor(stack_t *stack, int stack_capacity)
{
    // Error check
    if (stack == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(STACK_EXIT_CODES::PASSED_STACK_IS_NULLPTR);
        return EXIT_CODES::BAD_OBJECT_PASSED;
    }

    if (stack->data != NULL || stack->capacity != -1 || stack->size != -1)
    {
        PRINT_ERROR_TRACING_MESSAGE(STACK_EXIT_CODES::OLD_STACK_PASSED);
        return EXIT_CODES::BAD_OBJECT_PASSED;
    }

    // Count additional bytes needed for security fields
    int protection_capacity_increse = 0;
    IS_OK_W_EXIT(stackCapacitySecurityIncrease(stack, &protection_capacity_increse));

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

    // Fill stack structure
    stack->capacity = stack_capacity;
    stack->size = 0;

    // Poison allocated data fields
    IS_OK_W_EXIT(sprayPoisonOnData(stack));

    // Construct hasSum field
    #if defined(STACK_HASH) && STACK_HASH == 1
        IS_OK_W_EXIT(hashSumCtor(stack));
    #endif

    return EXIT_CODES::NO_ERRORS;
}

EXIT_CODES getNewReallocationCapacity(stack_t *stack, REALLOC_MODES mode, int *new_capacity)
{
    // Error check
    OBJECT_VERIFY(stack, stack);

    if (new_capacity == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::BAD_OBJECT_PASSED);
        return EXIT_CODES::BAD_OBJECT_PASSED;
    }

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

    // Error check
    OBJECT_VERIFY(stack, stack);

    return EXIT_CODES::NO_ERRORS;
}

EXIT_CODES stackReallocation(stack_t *stack, REALLOC_MODES mode)
{
    // Error check
    OBJECT_VERIFY(stack, stack);

    // Get new reallocation capacity
    int new_capacity = 0;
    IS_OK_W_EXIT(getNewReallocationCapacity(stack, mode, &new_capacity));

    int protection_capacity_increse = 0;
    #if defined(STACK_CANARY) && STACK_CANARY == 1
        protection_capacity_increse += (LEFT_CANARY_SIZE + RIGHT_CANARY_SIZE);
    #endif

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
    IS_OK_W_EXIT(sprayPoisonOnData(stack));

    // Update hash sum
    #if defined(STACK_HASH) && STACK_HASH == 1
        IS_OK_W_EXIT(hashSumCtor(stack));
    #endif

    // Error check
    OBJECT_VERIFY(stack, stack);

    return EXIT_CODES::NO_ERRORS;
}

EXIT_CODES stackPush(stack_t *stack, stackElem_t value)
{
    // Error check
    OBJECT_VERIFY(stack, stack);

    if (stack->size == stack->capacity)
    {
        IS_OK_W_EXIT(stackReallocation(stack, REALLOC_MODES::INCREASE));
    }

    stack->data[stack->size++] = value;

    // Update hash sum
    #if defined(STACK_HASH) && STACK_HASH == 1
        IS_OK_W_EXIT(hashSumCtor(stack));
    #endif

    // Error check
    OBJECT_VERIFY(stack, stack);

    return EXIT_CODES::NO_ERRORS;
}

EXIT_CODES stackPop(stack_t *stack, stackElem_t *popTo)
{
    // Error check
    OBJECT_VERIFY(stack, stack);

    if (stack->size < 1)
    {
        PRINT_ERROR_TRACING_MESSAGE(STACK_EXIT_CODES::BAD_STACK_SIZE);
        return EXIT_CODES::BAD_OBJECT_PASSED;
    }

    if (popTo == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::BAD_OBJECT_PASSED);
        return EXIT_CODES::BAD_OBJECT_PASSED;
    }

    if (stackReallocCoefficient * stack->size < stack->capacity)
    {
        IS_OK_W_EXIT(stackReallocation(stack, REALLOC_MODES::DECREASE));
    }

    if (popTo == DEFAULT_POPTO_VALUE)
    {
        --stack->size; 
    }
    else
    {
        *popTo = stack->data[--stack->size];
    }

    stack->data[--stack->size] = POISON;

    // Update hash sum
    #if defined(STACK_HASH) && STACK_HASH == 1
        IS_OK_W_EXIT(hashSumCtor(stack));
    #endif

    // Error check
    OBJECT_VERIFY(stack, stack);

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

            for (int element = 0; element < 8; ++element)
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

        #if defined(STACK_CANARY) && STACK_CANARY == 1
            fprintf(DEFAULT_ERROR_TRACING_STREAM, "\t\tHASH_SUM = %lld \n", stack->hashSum);
        #endif

        fprintf(DEFAULT_ERROR_TRACING_STREAM, "}\n");

        return EXIT_CODES::NO_ERRORS;
    }
#endif

EXIT_CODES stackDtor(stack_t *stack)
{
    // Error check
    OBJECT_VERIFY(stack, stack);

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