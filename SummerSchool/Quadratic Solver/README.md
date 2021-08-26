# What does this program do?
This program simply solves the quadratic equation.

# What is implemented?
* There is a quadratic solver and a unit testing program.\
* The unit_test program tests quadratic solver with randomly (see random.h, random.c) generated coefficients (see unit_test.h, unit_test.c).\
* The main engine of a quadratic solver is implemented in quadratic_equation.h and quadratic_equation.c.\
* Also there is an error handler (see error.h, error.c) that handles errors that may occur while using the program.\
* All constants are in one file (see constants.h) that can be easily changed (e.g. EPS, TOTAL_UNIT_TESTS and etc.).


# How to compile it?
For the unit_test mode (only tests are passed to the program, you can't type any values by hand) you need to uncomment the '#include "../include/unit_test.h"' line in the main.c file, then open the terminal in 'src' folder and run the following command:
```
gcc -o unit_test main.c error.c quadratic_equation.c random.c unit_test.c ../include/constants.h
```
A 'unit_test.exe' file will be created in the 'src' directory.\

Fot the normal mode (you can type coefficients by hand)  you need to comment the '#include "../include/unit_test.h"' line in the main.c file, then open the terminal in 'src' folder and run the following command:
```
gcc -o prog main.c error.c quadratic_equation.c random.c ../include/constants.h
```
A 'prog.exe' file will be created in the 'src' directory.

# Is there a documentation?
Yes, you can see the comments in the source code. There are comments that describe each function.
