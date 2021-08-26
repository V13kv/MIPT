#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <signal.h>

#include "../include/constants.h"
#include "../include/quadratic_equation.h"
#include "../include/unit_test.h"  // comment if you don't want to compile in unit test mode (only tests will be done)


int main(void)
{
    // Intorduction message
    startupMessage();

    #ifdef UNIT_TEST_H
        long int tests_passed = unitTest();  // Test our program using TOTAL_UNIT_TESTS
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
        
        int code = getQuadraticEquationCoefficients(&a, &b, &c);
        if (code != 0)
        {
            raise(code);
        }

        // Finding roots
        uint8_t roots_found = 0;
        double roots[2] = {0, 0};
        roots_found = solveQuadraticEquation(a, b, c, roots);

        // Printing roots
        printRoots(roots, roots_found);
    #endif  // UNIT_TEST_H

    system("pause");

    return 0;
}