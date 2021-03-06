#ifndef STACK_H
#define STACK_H

#include <cstddef>
#include <stdbool.h>

#define DEBUG_LEVEL 0
#include "../../debug/debug.h"
#include "settings.h"

#ifndef STACK_CANARY
    #define STACK_CANARY 0
#endif

#ifndef STACK_HASH
    #define STACK_HASH 0
#endif

#ifndef DEBUG_LEVEL
    #define DEBUG_LEVEL 0
#endif

enum class STACK_EXIT_CODES
{
    OLD_STACK_PASSED,
    BAD_STACK_SIZE,
    PASSED_STACK_IS_NULLPTR,
    BAD_REALLOC_MODE_PASSED,
    STACK_DATA_CANARY_IS_DAMAGED,
    STACK_STRUCTURE_CANARY_IS_DAMAGED,
    STACK_HASH_SUM_IS_DAMAGED,
    BAD_STACK_PASSED,
    ERROR_DURING_STACK_CTOR,
    ERROR_DURING_STACK_PUSH,
    ERROR_DURING_STACK_POP,
    ERROR_DURING_STACK_DTOR,
};

enum class REALLOC_MODES
{
    INCREASE,
    DECREASE
};

struct stack_t
{
    #if STACK_CANARY == 1
        const int canaryLeft = CANARY_STRUCT_VALUE;
    #endif

    stackElem_t *data = NULL;
    int capacity = -1;
    int size = -1;

    #if STACK_CANARY == 1
        const int canaryRight = CANARY_STRUCT_VALUE;
    #endif

    #if STACK_HASH == 1
        long long int hashSum = 0;
    #endif
};

#if DEBUG_LEVEL == 2
    EXIT_CODES stackDump(stack_t *stack);
#else
    #define stackDump(stack) EXIT_CODES::NO_ERRORS;
#endif

#if STACK_CANARY == 1  
    EXIT_CODES canaryCheck(stack_t *stack, bool *result);
    EXIT_CODES canaryCtor(stack_t *stack, int stack_capacity);
#endif

#if STACK_HASH == 1
    EXIT_CODES calculateStackHashSum(stack_t *stack, long long int *hashSum);
    EXIT_CODES stackHashCheck(stack_t *stack, bool *result);
#endif

#if STACK_CANARY == 1 || STACK_HASH == 1
    EXIT_CODES stackCapacityIncrease(stack_t *stack, int *add_bytes);
#else
    #define stackCapacityIncrease(stack, add_bytes)  EXIT_CODES::NO_ERRORS;
#endif

bool stackBasicCheck(stack_t *stack);
bool stackOk(stack_t *stack);
EXIT_CODES sprayPoisonOnData(stack_t *stack);
EXIT_CODES stackCtor(stack_t *stack, int stack_capacity = 8);
EXIT_CODES stackDtor(stack_t *stack);

EXIT_CODES getNewReallocationCapacity(stack_t *stack, REALLOC_MODES mode, int *new_capacity);
EXIT_CODES stackReallocation(stack_t *stack, REALLOC_MODES mode);

EXIT_CODES stackPush(stack_t *stack, stackElem_t value);

#define DEFAULT_POPTO_VALUE (stackElem_t *) 1
EXIT_CODES stackPop(stack_t *stack, stackElem_t *popTo = DEFAULT_POPTO_VALUE);

#endif  // STACK_H
