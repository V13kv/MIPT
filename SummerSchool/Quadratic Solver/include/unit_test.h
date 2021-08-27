#ifndef UNIT_TEST_H
#define UNIT_TEST_H


#include <stdint.h>
#include <stdbool.h>

int generateCoefficients(double *const a, double *const b, double *const c);
int areRootsCorrect(const double a, const double b, const double c, double *const roots, const int roots_found, bool *const roots_are_correct);
int unitTest(long int *const tests_passed);


#endif  // UNIT_TEST_H