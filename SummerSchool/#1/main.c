//TODO: move all declarations to .h files to beautify the code
//TODO: add namespaces
#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <signal.h>
#include <math.h>
#include <time.h>
#include <string.h>
//#include <float.h>  // Only used for DBL_EPSILON


const double EPS = 1e-6;//DBL_EPSILON
const long int TOTAL_UNIT_TESTS = 1483647;
const bool UNIT_TEST = false;  // if true than program will run in unit test mode
const double RAND_MAX_D = RAND_MAX;
const double MIN_DOUBLE_VALUE = -1000.0;
const double MAX_DOUBLE_VALUE = 1000.0;

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

enum UNITTEST_RESULTS
{
    WRONG_ANSWER,
    CORRECT_ANSWER,
};

// GENERAL FUNCTIONS
static void startupMessage();
void errorHandler(const int error_code, const char *const error_function_name);
void getCoefficients(double *const a, double *const b, double *const c);
int solveTheEquation(const double a, const double b, const double c, double *const roots);
void printRoots(const double *const roots, const uint8_t total_roots);
// END OF GENERAL FUNCTIONS

// FUNCTIONS FOR UNIT TEST
double get_rand_0_1(void);
double get_rand_in_range(const double min, const double max);
void generateCoefficients(double *const a, double *const b, double *const c);
bool areRootsCorrect(const double a, const double b, const double c, double *const roots, const uint8_t roots_found);
long int unitTest(void);
// END OF UNIT TEST FUNCTIONS


int main(void)
{
    // Intorduction message
    startupMessage();

    // Choosing the way relying on the UNIT_TEST variable
    if (UNIT_TEST)
    {
        long int tests_passed = unitTest();
        if (tests_passed == TOTAL_UNIT_TESTS)
        {
            printf("[+] All %d tests are successfully passed!\n", tests_passed);
        }      
        else
        {
            printf("[!] Only %d tests are successfully passed!\n", tests_passed);
        }
    }
    else
    {
        // Getting input data
        double a = 0, b = 0, c = 0;
        getCoefficients(&a, &b, &c);

        // Finding roots
        uint8_t roots_found = 0;
        double roots[2] = {0, 0};
        roots_found = solveTheEquation(a, b, c, roots);

        // Printing roots
        printRoots(roots, roots_found);
    }

    return 0;
}

/*
 * Function:  get_rand_0_1.
 * What does it do?: Generates random number between 0 and 1.
 * Returns: randomly generated number.
 */
inline double get_rand_0_1(void)
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
inline double get_rand_in_range(const double min, const double max)
{
    return get_rand_0_1() * (max - min) + min;
}

/*
 * Function:  generateCoefficients.
 * What does it do?: Randomly generates coefficients (a, b and c).
 * 
 * Arguments:
 *      a, b, c: coefficients in a quadratic equation of the form ax^2+bx+c=0
 */
void generateCoefficients(double *const a, double *const b, double *const c)
{
    if (a == NULL || b == NULL || c == NULL)
    {
        errorHandler(BAD_POINTERS_PASSED, __func__);
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

    // Define variables for the while cycle
    double a, b, c;  // Our coefficients
    double roots[2]; // The actual roots of the equation (the will be no more than 2)
    uint8_t roots_found;  // The amount of root that solveTheEquation() function will find
    long int tests_passed = 0;

    // Main 'TESTING' cycle
    while (tests_passed < TOTAL_UNIT_TESTS)
    {
        // Initializing variables (but it is not important actually)
        a = 0, b = 0, c = 0, roots_found = 0;
        memset(roots, 0, sizeof(double) * 2);  // Clearing the roots array

        // Randomly generate coefficients
        generateCoefficients(&a, &b, &c);

        // Solving the equation (using our algorithm)
        roots_found = solveTheEquation(a, b, c, roots);

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
                printf("[!] Answer is incorrect for the roots: ");
                printRoots(roots, roots_found);
            }
        }
    }

    return tests_passed;
}

inline void startupMessage()
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
//TODO: maybe write normal errorHandler (but it is not necessarily)
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
    else if (D > 0)
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
        for (uint8_t root_num = 0; root_num < total_roots; ++root_num)
        {
            printf("Root #%u: %f; ", root_num, roots[root_num]);
        }
    }   
    printf("\n");
}