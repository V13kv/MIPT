#include <stdio.h>

#include "../include/error.h"
#include "../include/constants.h"


/*
 * Function:  errorHandler.
 * What does it do?: Handles errors.
 * Returns: the ERROR_CODE of an error.
 * 
 * Arguments:
 *      error_code: number of an error_code (see ERROR_CODES for all errors)
 *      error_function_name: the name of the function that called this function
 */
int errorHandler(const int error_code, const char *const error_function_name)
{
    switch (error_code)
    {
        case BAD_COEFFICIENT:
            printf("[ERROR] Bad coefficient typed in %s function! You should enter a number!\n", error_function_name);
            return BAD_COEFFICIENT;
        case BAD_POINTERS_PASSED:
            printf("[ERROR] Null ptr's passed to a %s function\n", error_function_name);
            return BAD_POINTERS_PASSED;
        default:
            printf("[ERROR] Unknown error in function %s\n", error_function_name);
            return UNKNOWN_ERROR;
    }
}