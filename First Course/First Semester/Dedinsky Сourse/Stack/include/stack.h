#ifndef STACK_H
#define STACK_H

#include <cstddef>
#include <stdbool.h>

#include "settings.h"
#include "colors.h"

enum class EXIT_CODES
{
    NO_ERRORS,
    OLD_STACK_PASSED,
    BAD_STD_FUNC_RESULT,
    BAD_STACK_PASSED,
    BAD_REALLOC_MODE_PASSED
};

enum class REALLOC_MODES
{
    INCREASE,
    DECREASE
};

#if defined STACK_DEBUG_MODE && STACK_DEBUG_MODE >= 1

#define VALUE_CODE_TO_STR(expression) expression ? GREEN"OK"RESET : RED"ERROR"RESET
#define STRINGIFY(object) #object

#define IS_OK(exit_code)                                                    \
    do                                                                      \
    {                                                                       \
        if (exit_code != EXIT_CODES::NO_ERRORS)                             \
        {                                                                   \
            fprintf(DEFAULT_ERROR_TRACING_STREAM,                           \
                    RED"[ERROR] "RESET"Error in %s(%d) -> %s() -> %s;\n",   \
                    __FILE__, __LINE__, __func__, #exit_code);              \
            exit(EXIT_FAILURE);                                             \
        }                                                                   \
    } while (0)

#define PRINT_ERROR_TRACING_MESSAGE(errorMsg)                                   \
    do                                                                          \
    {                                                                           \
        fprintf(DEFAULT_ERROR_TRACING_STREAM,                                   \
                RED"[INTERNAL_ERROR] "RESET"Error in %s(%d) -> %s() -> %s\n",   \
                __FILE__, __LINE__, __func__, #errorMsg                         \
        );                                                                      \
    } while (0)

#define STACK_VERIFY(stack)                                                     \
    do                                                                          \
    {                                                                           \
        if (!isStackOk(stack))                                                  \
        {                                                                       \
            PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::BAD_STACK_PASSED);          \
            stackDump(stack);                                                   \
            return EXIT_CODES::BAD_STACK_PASSED;                                \
        }                                                                       \
    } while (0)
#else
    #define IS_OK(exit_code)
    #define PRINT_ERROR_TRACING_MESSAGE(errorMsg)
    #define STACK_VERIFY(stack)
#endif

struct stack_t
{
    stackElem_t *data = NULL;
    int capacity = -1;
    int size = -1;
};

#if defined STACK_DEBUG_MODE && STACK_DEBUG_MODE == 2
    EXIT_CODES stackDump(stack_t *stack);
#else
    #define stackDump(smth)
#endif

bool isStackOk(stack_t *stack);
EXIT_CODES stackCtor(stack_t *stack, int stack_capacity = 8);
EXIT_CODES stackDtor(stack_t *stack);

EXIT_CODES getNewReallocationCapacity(stack_t *stack, REALLOC_MODES mode, int *new_capacity);
EXIT_CODES stackReallocation(stack_t *stack, REALLOC_MODES mode);

EXIT_CODES stackPush(stack_t *stack, stackElem_t value);
EXIT_CODES stackPop(stack_t *stack, stackElem_t *popTo = NULL);



#endif  // STACK_H
