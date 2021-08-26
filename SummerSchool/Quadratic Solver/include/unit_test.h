#ifndef UNIT_TEST_H
#define UNIT_TEST_H


#include <stdint.h>
#include <stdbool.h>

int generateCoefficients(double *const a, double *const b, double *const c);
bool areRootsCorrect(const double a, const double b, const double c, double *const roots, const uint8_t roots_found);
long int unitTest(void);


#endif  // UNIT_TEST_H