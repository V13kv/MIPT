#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../include/unit_test.h"
#include "../include/constants.h"
#include "../include/error.h"
#include "../include/random.h"
#include "../include/quadratic_equation.h"


/*
 * Function:  generateCoefficients.
 * What does it do?: Randomly generates coefficients (a, b and c).
 * 
 * Arguments:
 *      a, b, c: coefficients in a quadratic equation of the form ax^2+bx+c=0
 */
int generateCoefficients(double *const a, double *const b, double *const c)
{
    if (a == NULL || b == NULL || c == NULL)
    {
        return errorHandler(BAD_POINTERS_PASSED, __func__);
    }

    *a = get_rand_in_range(MIN_DOUBLE_VALUE, MAX_DOUBLE_VALUE);
    *b = get_rand_in_range(MIN_DOUBLE_VALUE, MAX_DOUBLE_VALUE);
    *c = get_rand_in_range(MIN_DOUBLE_VALUE, MAX_DOUBLE_VALUE);
}

/*
 * Function:  areRootsCorrect.
 * What does it do?: Determines whether the roots are correct.
 * Returns: 0 if the roots do not satisfy the quadratic equation
 *          1 if the roots satisfy the quadratic equation.
 * 
 * Arguments:
 *      a, b, c: coefficients in a quadratic equation of the form ax^2+bx+c=0
 *      roots: array that saves found roots
 *      roots_found: total amount of roots to check
 */
bool areRootsCorrect(const double a, const double b, const double c, double *const roots, const uint8_t roots_found)
{
    for (int root_num = 0; root_num < roots_found; ++root_num)
    {
        double check_res = a*roots[root_num]*roots[root_num] + b*roots[root_num] + c;
        if (fabs(check_res) < EPS)
            continue;
        else
            return WRONG_ANSWER;
    }

    return CORRECT_ANSWER;  
}

/*
 * Function:  unitTest.
 * What does it do?: Tests the program TOTAL_UNIT_TESTS times.
 * Returns: total amount of passed tests
 */
long int unitTest(void)
{
    // Setting the seed
    srand(time(NULL));

    // Main 'TESTING' cycle
    long int tests_passed = 0;
    while (tests_passed < TOTAL_UNIT_TESTS)
    {
        double a = 0, b = 0, c = 0;  // Our coefficients
        double roots[2] = {0, 0};  // The actual roots of the equation (the will be no more than 2)
        uint8_t roots_found = 0;  // The amount of root that solveQuadraticEquation() function will find

        // Randomly generate coefficients
        generateCoefficients(&a, &b, &c);

        // Solving the equation (using our algorithm)
        roots_found = solveQuadraticEquation(a, b, c, roots);

        // Checking our roots for the correctness
        if (roots_found)  // if there are any roots
        {
            if (areRootsCorrect(a, b, c, roots, roots_found))
            {
                // Uncomment 2 rows below if you want to see the roots which satisfy the quadratic equation
                //printf("[CORRECT] Answer is correct for the roots: ");
                //printRoots(roots, roots_found);

                ++tests_passed;
            }
            else
            {
                //TODO: add printing coefficients
                printf("[!] Answer is incorrect for the equation: TODO:");
                printf("[!] Answer is incorrect for the roots: ");
                printRoots(roots, roots_found);
            }
        }
    }

    return tests_passed;
}
