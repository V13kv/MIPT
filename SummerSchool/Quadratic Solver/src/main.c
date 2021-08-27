#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <signal.h>

#include "../include/error.h"
#include "../include/constants.h"
#include "../include/quadratic_equation.h"
//#include "../include/unit_test.h"  // comment if you don't want to compile in unit test mode (only tests will be done)


int main(void)
{
    // Intorduction message
    startupMessage();

    #ifdef UNIT_TEST_H
        long int tests_passed = 0;
        int exit_code = unitTest(&tests_passed);  // Test our program using TOTAL_UNIT_TESTS
        functionErrorMonitor(exit_code);

        if (tests_passed == TOTAL_UNIT_TESTS)
        {
            printf("[+] All %d tests are successfully passed!\n", tests_passed);
        }      
        else
        {
            printf("[!] Only %d tests are successfully passed!\n", tests_passed);
        }
    #else
        // Getting input data
        double a = 0, b = 0, c = 0;

        int exit_code = getQuadraticEquationCoefficients(&a, &b, &c);
        functionErrorMonitor(exit_code);

        // Finding roots
        int roots_found = 0;
        double roots[2] = {0, 0};

        exit_code = solveQuadraticEquation(a, b, c, roots, &roots_found);
        functionErrorMonitor(exit_code);

        // Printing roots
        exit_code = printRoots(roots, roots_found);
        functionErrorMonitor(exit_code);
    #endif  // UNIT_TEST_H

    system("pause");

    return 0;
}