// TODO: test program, automate process, write program that tests this program, just bruteforce (randomly)
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <signal.h>
#include <math.h>


const double EPS = 1e-10;

enum ERROR_CODES
{
    UNKNOWN_ERROR = SIGTERM,
    BAD_POINTERS_PASSED = SIGTERM,
    BAD_COEFFICIENT
};

enum TOTAL_ROOTS
{
    NO_ROOTS,
    ONE_ROOT,
    TWO_ROOTS
};

static void startupMessage();
void errorHandler(const int error_code, const char *const error_function_name);
void getCoefficients(double *const a, double *const b, double *const c);
int solveTheEquation(const double a, const double b, const double c, double *const roots);
void printRoots(const double *const roots, const uint8_t total_roots);

int main(void)
{
    // Intorduction message
    startupMessage();

    // Getting input data
    double a = 0, b = 0, c = 0;
    getCoefficients(&a, &b, &c);

    // Finding roots
    uint8_t roots_found = 0;
    double roots[2] = {0, 0};
    roots_found = solveTheEquation(a, b, c, roots);

    // Printing roots
    printRoots(roots, roots_found);

    return 0;
}

static void startupMessage()
{
    printf("---Program to find the roots of an equation---\n");
}

/*
 * Function:  errorHandler.
 * What does it do?: Handles errors.
 * Returns: the number of solutions for an equation.
 * 
 * Arguments:
 *      error_code: number of an error_code (see ERROR_CODES for all errors)
 *      error_function_name: the name of the function that called this function
 */
void errorHandler(const int error_code, const char *const error_function_name)
{
    switch (error_code)
    {
        case BAD_COEFFICIENT:
            printf("[ERROR] Bad coefficient typed in %s function! You should enter a number!\n", error_function_name);
            break;
        case BAD_POINTERS_PASSED:
            printf("[ERROR] Null ptr's passed to a %s function\n", error_function_name);
            exit(BAD_POINTERS_PASSED);
        default:
            printf("[ERROR] Unknown error in function %s\n", error_function_name);
            exit(UNKNOWN_ERROR);
    }
}

/*
 * Function:  getCoefficients.
 * What does it do?: Gets the coefficients from the user.
 * 
 * Arguments:
 *      a, b, c: coefficients in a quadratic equation of the form ax^2+bx+c=0
 */
void getCoefficients(double *const a, double *const b, double *const c)
{
    if (a == NULL || b == NULL || c == NULL)
    {
        errorHandler(BAD_POINTERS_PASSED, __func__);
    }

    while (scanf("%lf %lf %lf", a, b, c) != 3)
    {
        errorHandler(BAD_COEFFICIENT, __func__);
        fflush(stdin);
    }
}

/*
 * Function:  solveTheEquation.
 * What does it do?: Solves the equations with a given coefficients.
 * Returns: the number of solutions for an equation.
 * 
 * Arguments:
 *      a, b, c: coefficients in a quadratic equation of the form ax^2+bx+c=0
 *      roots: array that saves found roots
 */
int solveTheEquation(const double a, const double b, const double c, double *const roots)
{
    const double D = b*b - 4*a*c;

    if (fabs(D) < EPS)
    {
        const double root = ( b == 0 ? 0 : -b/(2*a) );  // Checking whether the root is 0 (if so then beautify output (print 0, not -0))
        roots[0] = root;

        return ONE_ROOT;
    }
    else if (fabs(D) > EPS)
    {
        const double sD = sqrt(D);
        const double root1 = (-b + sD) / (2*a);
        const double root2 = (-b - sD) / (2*a);
        
        roots[0] = root1;
        roots[1] = root2;

        return TWO_ROOTS;
    }
    else
    {
       return NO_ROOTS;
    }
}

/*
 * Function:  printRoots.
 * What does it do?: Prints roots of the equation.
 * 
 * Arguments:
 *      roots: the array that contains the roots to print
 *      total_roots: the total amount of roots in the equation
 */
void printRoots(const double *const roots, const uint8_t total_roots)
{
    if (total_roots == 0)
    {
        printf("There are no roots\n");
    }
    else
    {
        for (uint8_t root_number = 0; root_number < total_roots; ++root_number)
        {
            printf("Root #%u: %f\n", root_number, roots[root_number]);
        }
    }   
}