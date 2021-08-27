#ifndef QUADRATIC_EQUATION_H
#define QUADRATIC_EQUATION_H


#include <stdint.h>

int getQuadraticEquationCoefficients(double *const a, double *const b, double *const c);
int solveQuadraticEquation(const double a, const double b, const double c, double *const roots, int *const roots_found);
int printRoots(const double *const roots, const int total_roots);
void startupMessage();


#endif  // QUADRATIC_EQUATION_H