#ifndef ASM_SETTINGS_H
#define ASM_SETTINGS_H

#define COMMAND_FORMAT                  "%[a-z]%n %n%s%n"
#define MNEMONICS_FORMAT                "%[a-z] %n"
#define REGISTER_FORMAT                 "%2[a-z]%n"
#define IMMEDIATE_VALUE_FORMAT          "%lf%n"

const int MAX_MNEMONICS_STR_LENGTH        = 5;
const int MAX_INSTRUCTION_ARGS_STR_LEN    = 50;
const int MAX_ARGUMENTS_PER_COMMAND       = 2;
const int MAX_ARGUMENT_STR_LENGTH         = 50;
const int MAX_REGISTER_STR_LENGTH         = 3;
const int NO_ARGUMENTS                    = 0;
const int MAX_ENCODED_COMMAND_LENGTH      = 100;


#endif  // ASM_SETTINGS_H
