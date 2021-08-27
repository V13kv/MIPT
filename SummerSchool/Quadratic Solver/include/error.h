#ifndef ERROR_H
#define ERROR_H


void printErrorMessage(const int error_code, const char *const error_function_name);
void functionErrorMonitor(const int exit_code);


#endif  // ERROR_H