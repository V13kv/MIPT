#include <cstddef>

#include "../../debug/debug.h"
#include "../include/hash.h"


EXIT_CODES calculateHashSum(void *object, unsigned long long int size, long long int *hashSum)
{
    // Error check
    if (object == NULL || hashSum == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::PASSED_OBJECT_IS_NULLPTR);
        return EXIT_CODES::PASSED_OBJECT_IS_NULLPTR;
    }

    // Calculation of hash sum
    *hashSum = 0;
    char *byteObject = (char *) object;
    for (unsigned long long byte = 0; byte < size; ++byte)
    {
        (*hashSum) += byteObject[byte] * byte;
    }

    return EXIT_CODES::NO_ERRORS;
}
