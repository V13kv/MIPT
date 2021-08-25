#include <stdio.h>
#include <math.h>
#include <stdlib.h>


void getCoefficients(double *p_a, double *p_b, double *p_c)
{
    if (scanf("%lf %lf %lf", p_a, p_b, p_c) != 3)
    {
        printf("[!] Check input data\n");
        exit(1);
    }
}

void startupMessage()
{
    printf("---Program to find the roots of an equation---\n");
}

int main()
{
    startupMessage();

    // Get input data
    double a, b, c;
    getCoefficients(&a, &b, &c);

    // Calculations to find roots
    int D = b*b - 4*a*c;
    if (D < 0)
    {
        printf("[!] There are no roots");
    }
    else if (D == 0)
    {
        printf("Root found: %f", b == 0 ? 0 : -b/(2*a));
    }
    else
    {
        printf(
            "Root #1: %f\n"
            "Root #2: %f", 
            ( -b+sqrt(D) ) / (2*a),
            ( -b-sqrt(D) ) / (2*a)
        );
    }


    return 0;
}