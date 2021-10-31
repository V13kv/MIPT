#ifndef DEBUG_H
#define DEBUG_H

#include <stdio.h>

#include "../colors/colors.h"

#define DEFAULT_ERROR_TRACING_STREAM    stderr

// Debug level must be defined in your file before including this library
#pragma region DEBUG_LEVELS
    //#define STACK_DEBUG_LEVEL 1  // Minimal debug info (only messages are shown)
    //#define STACK_DEBUG_LEVEL 2  // Maximum debug info (messages and dumps are shown)
#pragma endregion DEBUG_LEVELS

// Universal exit codes
enum class EXIT_CODES
{
    NO_ERRORS,
    BAD_OBJECT_PASSED,
    PASSED_OBJECT_IS_NULLPTR,
    BAD_STD_FUNC_RESULT,
};

#if defined(DEBUG_LEVEL) && DEBUG_LEVEL >= 1

    #define VALUE_CODE_TO_STR(expression)  ( !!(expression) ? GREEN"OK"RESET : RED"ERROR"RESET )
    #define STRINGIFY(object) #object
 
    #define IS_OK(function, exitOnError)                                                \
        do                                                                              \
        {                                                                               \
            EXIT_CODES exit_code = function;                                            \
            if (exit_code != EXIT_CODES::NO_ERRORS)                                     \
            {                                                                           \
                fprintf(DEFAULT_ERROR_TRACING_STREAM,                                   \
                        RED"[ERROR] "RESET"Error in %s(%d) -> %s() -> %s(%d);\n",       \
                        __FILE__, __LINE__, __func__, #function, ((int) exit_code));    \
                if (exitOnError)                                                        \
                {                                                                       \
                    exit(EXIT_FAILURE);                                                 \
                }                                                                       \
            }                                                                           \
        } while (0)

    #define IS_OK_WO_EXIT(function) IS_OK(function, false)

    #define IS_OK_W_EXIT(function) IS_OK(function, true)

    #define PRINT_ERROR_TRACING_MESSAGE(errorMsg)                                       \
        do                                                                              \
        {                                                                               \
            fprintf(DEFAULT_ERROR_TRACING_STREAM,                                       \
                    RED"[INTERNAL_ERROR] "RESET"Error in %s(%d) -> %s() -> %s(%d)\n",   \
                    __FILE__, __LINE__, __func__, #errorMsg, (int) errorMsg             \
            );                                                                          \
        } while (0)

    #define OBJECT_VERIFY(object, type)                                                 \
        do                                                                              \
        {                                                                               \
            if (!type##Ok(object))                                                      \
            {                                                                           \
                type##Dump(object);                                                     \
                PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::BAD_OBJECT_PASSED);             \
                return EXIT_CODES::BAD_OBJECT_PASSED;                                   \
            }                                                                           \
        } while (0)

#else

    #define IS_OK(function, exitOnError)    function
    #define IS_OK_WO_EXIT(function)         function
    #define IS_OK_W_EXIT(function)          function
    #define PRINT_ERROR_TRACING_MESSAGE(error_code)
    #define OBJECT_VERIFY(object, type)

#endif


#endif  // DEBUG_H
