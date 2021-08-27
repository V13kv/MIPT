#include <stdio.h>

#include "../include/error.h"
#include "../include/constants.h"


/*
 * Function:  printErrorMessage.
 * What does it do?: Printing human readable error message.
 * 
 * Arguments:
 *      error_code: number of an error_code (see FUNCTION_EXIT_CODES for all errors)
 *      error_function_name: the name of the function that called this function
 */
void printErrorMessage(const int error_code, const char *const error_function_name)
{
    switch (error_code)
    {
        case BAD_COEFFICIENT:
            printf("[ERROR] Bad coefficient typed in %s function! You should enter a number!\n", error_function_name);
            break;
        case BAD_POINTERS_PASSED:
            printf("[ERROR] Null ptr passed to a %s function\n", error_function_name);
            break;
        default:
            printf("[ERROR] Unknown error in function %s\n", error_function_name);
            break;
    }
}


/*
 * Function:  functionErrorMonitor.
 * What does it do?: Handles all errors (just raising the error).
 * 
 * Arguments:
 *      exit_code: number of an function error code (see FUNCTION_EXIT_CODES for all codes)
 */
void functionErrorMonitor(const int exit_code)
{
    if (exit_code != NO_ERRORS)
    {
        raise(exit_code);
    }
}