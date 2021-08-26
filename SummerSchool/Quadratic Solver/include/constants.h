#ifndef CONSTANTS_H
#define CONSTANTS_H


#include <signal.h>
#include <stdlib.h>
//#include <float.h>  // if you want to use DBL_EPSILON to improve quadratic equation unit testing accuracy

#ifdef DBL_EPSILON
    static const double EPS = DBL_EPSILON;
#else
    static const double EPS = 1e-6;
#endif  // DBL_EPSILON

static const double MIN_DOUBLE_VALUE = -1000.0;
static const double MAX_DOUBLE_VALUE = 1000.0;
static const long int TOTAL_UNIT_TESTS = 10;
static const double RAND_MAX_D = RAND_MAX;

enum ERROR_CODES
{
    BAD_COEFFICIENT,
    UNKNOWN_ERROR = SIGTERM,
    BAD_POINTERS_PASSED = SIGTERM
};

enum UNITTEST_RESULTS
{
    WRONG_ANSWER,
    CORRECT_ANSWER,
};

enum TOTAL_ROOTS
{
    NO_ROOTS,
    ONE_ROOT,
    TWO_ROOTS
};


#endif  // CONSTANTS_H