#include <stdlib.h>
#include <stdio.h>
#include <math.h>


void startupMessage();
void errorHandler(int error_code, char *error_function_name);
void getCoefficients(double *a, double *b, double *c);
void solveTheEquation(double a, double b, double c);

enum ERROR_CODES
{
    UNKNOWN_ERROR,
    BAD_POINTERS_PASSED,
    BAD_COEFFICIENT
};

int main(void)
{
    // Intorduction message
    startupMessage();

    // Get input data
    double a = 0, b = 0, c = 0;
    getCoefficients(&a, &b, &c);

    // Calculations to find roots
    solveTheEquation(a, b, c);

    return 0;
}

void startupMessage()
{
    printf("---Program to find the roots of an equation---\n");
}

void errorHandler(int error_code, char *error_function_name)
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

void getCoefficients(double *a, double *b, double *c)
{
    if (a == NULL || b == NULL || c == NULL)
    {
        errorHandler(BAD_POINTERS_PASSED, __func__);
    }

    while (scanf("%lf %lf %lf", a, b, c) != 3)
    {
        errorHandler(BAD_COEFFICIENT, "getCoefficients()");
        fflush(stdin);
    }
}

void solveTheEquation(double a, double b, double c)
{
    double D = b*b - 4*a*c;
    
    double eps = 1e-10;
    if (D - eps < 0)
    {
        printf("There are no roots\n");
    }
    else if (D - eps > 0)
    {
        double sD = sqrt(D);
        double root1 = (-b + sD) / (2*a);
        double root2 = (-b - sD) / (2*a);
        
        printf("Root #1: %f\n", root1);
        printf("Root #2: %f\n", root2);
    }
    else
    {
        double root = ( b == 0 ? 0 : -b/(2*a) );  // Checking whether the root is 0 (if so then beautify output (print 0, not -0))
        printf("Root found: %f\n", root);
    }
}