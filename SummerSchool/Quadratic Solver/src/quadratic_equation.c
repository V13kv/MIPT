#include <math.h>
#include <stdio.h>

#include "../include/quadratic_equation.h"
#include "../include/constants.h"
#include "../include/error.h"


/*
 * Function:  getQuadraticEquationCoefficients.
 * What does it do?: Gets the coefficients from the user.
 * Returns: the function exit status.
 * 
 * Arguments:
 *      a, b, c: coefficients in a quadratic equation of the form ax^2+bx+c=0
 */
int getQuadraticEquationCoefficients(double *const a, double *const b, double *const c)
{
    if (a == NULL || b == NULL || c == NULL)
    {
        printErrorMessage(BAD_POINTERS_PASSED, __func__);
        return BAD_POINTERS_PASSED;
    }

    while (scanf("%lf %lf %lf", a, b, c) != 3)
    {
        printErrorMessage(BAD_COEFFICIENT, __func__);
        fflush(stdin);
    }

    return NO_ERRORS;
}

/*
 * Function:  solveQuadraticEquation.
 * What does it do?: Solves the equations with a given coefficients.
 * Returns: the function exit status.
 * 
 * Arguments:
 *      a, b, c: coefficients in a quadratic equation of the form ax^2+bx+c=0
 *      roots: array that saves found roots
 *      roots_found: total amount of roots found in the solution of the given equations
 */
int solveQuadraticEquation(const double a, const double b, const double c, double *const roots, int *const roots_found)
{
    if (roots == NULL || roots_found == NULL)
    {
        printErrorMessage(BAD_POINTERS_PASSED, __func__);
        return BAD_POINTERS_PASSED;
    }

    const double D = b*b - 4*a*c;
    if (fabs(D) < EPS)
    {
        const double root = ( b == 0 ? 0 : -b / (2 * a) );  // Checking whether the root is 0 (if so then beautify output (print 0, not -0))
        roots[0] = root;
        roots[1] = root;

        *roots_found = ONE_ROOT;
    }
    else if (D > 0)
    {
        const double sD = sqrt(D);
        const double root1 = (-b + sD) / (2 * a);
        const double root2 = (-b - sD) / (2 * a);
        
        roots[0] = root1;
        roots[1] = root2;
        
        *roots_found = TWO_ROOTS;
    }
    else
    {
       *roots_found = NO_ROOTS;
    }

    return NO_ERRORS;
}

/*
 * Function:  printRoots.
 * What does it do?: Prints roots of the equation.
 * Returns: the function exit status.
 * 
 * Arguments:
 *      roots: the array that contains the roots to print
 *      total_roots: the total amount of roots in the equation
 */
int printRoots(const double *const roots, const int total_roots)
{
    if (roots == NULL)
    {
        printErrorMessage(BAD_POINTERS_PASSED, __func__);
        return BAD_POINTERS_PASSED;
    }

    if (total_roots == 0)
    {
        printf("There are no roots\n");
    }
    else
    {
        for (uint8_t root_num = 0; root_num < total_roots; ++root_num)
        {
            printf("Root #%u: %f; ", root_num, roots[root_num]);
        }
    }   
    printf("\n");

    return NO_ERRORS;
}

/*
 * Function:  startupMessage.
 * What does it do?: Prints the introduction message.
 */
void startupMessage()
{
    printf("---Program to find the roots of an equation---\n");
}