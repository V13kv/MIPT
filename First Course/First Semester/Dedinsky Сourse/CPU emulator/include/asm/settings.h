#ifndef ASM_SETTINGS_H
#define ASM_SETTINGS_H


#define COMMAND_FORMAT                  "%[a-z]%n %n%s%n"
#define MNEMONICS_FORMAT                "%[a-z] %n"
#define REGISTER_FORMAT                 "%2[a-z]%n"
#define IMMEDIATE_VALUE_FORMAT          "%lf%n"

#define MAX_MNEMONICS_STR_LENGTH        5
#define MAX_INSTRUCTION_ARGS_STR_LEN    50
#define MAX_ARGUMENTS_PER_COMMAND       2
#define MAX_ARGUMENT_STR_LENGTH         50

#define OPDEF(opName, ...) #opName

    const char *MNEMONICS_TABLE[] = {
        #include "../opdefs.h"
    };

    const int MNEMONICS_TABLE_LENGTH = sizeof(MNEMONICS_TABLE) / sizeof(MNEMONICS_TABLE[0]);

#undef OPDEF

#define OPDEF(opName, opcode, ...) opcode

    const int OPCODES_TABLE[] {
        #include "../opdefs.h"
    };

#undef OPDEF

#define REGDEF(regName, regOpcode, ...) \
    {                                   \
        #regName,                       \
        regOpcode                       \
    },

    struct register_t
    {
        char *name;
        int opcode;
    };

    const register_t REGISTERS_TABLE[] = {
        #include "../registers.h"
    };

    const int REGISTERS_TABLE_LENGTH = sizeof(REGISTERS_TABLE) / sizeof(REGISTERS_TABLE[0]);

#undef REGDEF


#endif  // ASM_SETTINGS_H
