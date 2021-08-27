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
 * Returns: the function exit status.
 * 
 * Arguments:
 *      a, b, c: coefficients in a quadratic equation of the form ax^2+bx+c=0
 */
int generateCoefficients(double *const a, double *const b, double *const c)
{
    if (a == NULL || b == NULL || c == NULL)
    {
        printErrorMessage(BAD_POINTERS_PASSED, __func__);
        return BAD_POINTERS_PASSED;
    }

    *a = get_rand_in_range(MIN_DOUBLE_VALUE, MAX_DOUBLE_VALUE);
    *b = get_rand_in_range(MIN_DOUBLE_VALUE, MAX_DOUBLE_VALUE);
    *c = get_rand_in_range(MIN_DOUBLE_VALUE, MAX_DOUBLE_VALUE);

    return NO_ERRORS;
}

/*
 * Function: areRootsCorrect.
 * What does it do?: Determines whether the roots are correct.
 * Returns: the function exit status.
 * 
 * Arguments:
 *      a, b, c: coefficients in a quadratic equation of the form ax^2+bx+c=0
 *      roots: array that saves found roots
 *      roots_found: total amount of roots to check
 *      roots_are_correct: whether the roots are satisfy to the given equation or not
 */
int areRootsCorrect(const double a, const double b, const double c, double *const roots, const int roots_found, bool *const roots_are_correct)
{
    if (roots == NULL || roots_are_correct == NULL)
    {
        printErrorMessage(BAD_POINTERS_PASSED, __func__);
        return BAD_POINTERS_PASSED;
    }

    for (int root_num = 0; root_num < roots_found; ++root_num)
    {
        double check_res = a * roots[root_num] * roots[root_num] + b * roots[root_num] + c;
        if (fabs(check_res) < EPS)
            continue;
        else
            *roots_are_correct = false;
    }

    *roots_are_correct = true;
    return NO_ERRORS;
}

/*
 * Function:  unitTest.
 * What does it do?: Tests the program TOTAL_UNIT_TESTS times.
 * Returns: the function exit status.
 * 
 * Arguments:
 *      tests_passed: total amount of passed tests
 */
int unitTest(long int *const tests_passed)
{
    if (tests_passed == NULL)
    {
        printErrorMessage(BAD_POINTERS_PASSED, __func__);
        return BAD_POINTERS_PASSED;
    }

    // Setting the seed
    srand(time(NULL));

    // Main 'TESTING' cycle
    long int tests_failed = 0;
    while (*tests_passed + tests_failed < TOTAL_UNIT_TESTS)
    {
        double a = 0, b = 0, c = 0; // Our coefficients
        double roots[2] = {0, 0};   // The actual roots of the equation (the will be no more than 2)
        int roots_found = 0;    // The amount of root that solveQuadraticEquation() function will find

        // Randomly generate coefficients
        int exit_code = generateCoefficients(&a, &b, &c);
        if (exit_code != NO_ERRORS)
        {
            return exit_code;
        }

        // Solving the equation (using our algorithm)
        exit_code = solveQuadraticEquation(a, b, c, roots, &roots_found);
        if (exit_code != NO_ERRORS)
        {
            return exit_code;
        }

        // Checking our roots for the correctness
        if (roots_found) // if there are any roots
        {
            bool rootsAreCorrect = false;
            exit_code = areRootsCorrect(a, b, c, roots, roots_found, &rootsAreCorrect);
            if (exit_code != NO_ERRORS)
            {
                return exit_code;
            }

            if (rootsAreCorrect)
            {
                ++(*tests_passed);
            }
            else
            {
                printf("[!] Answer is incorrect for the equation: %fx**2 + %f*x + %f = 0 with roots: ", a, b, c);
                
                exit_code = printRoots(roots, roots_found);
                if (exit_code != NO_ERRORS)
                {
                    return exit_code;
                }

                ++tests_failed;
            }
        }
    }

    return NO_ERRORS;
}