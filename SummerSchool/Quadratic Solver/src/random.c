#include <stdlib.h>

#include "../include/random.h"
#include "../include/constants.h"

/*
 * Function:  get_rand_0_1.
 * What does it do?: Generates random number between 0 and 1.
 * Returns: randomly generated number.
 */
double get_rand_0_1(void)
{
    return rand() / RAND_MAX_D;
}

/*
 * Function:  get_rand_in_range.
 * What does it do?: Generates random number in the specified range.
 * Returns: randomly generated number.
 * 
 * Arguments:
 *      min: the left end of the line
 *      max: the right end of the line
 */
double get_rand_in_range(const double min, const double max)
{
    return get_rand_0_1() * (max - min) + min;
}