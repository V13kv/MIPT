#ifndef COMMANDS_H
#define COMMANDS_H


// TODO: void * (for double, int etc argument support)
struct command_t
{
    char *mnemonics = NULL;
    int opcode = -1;
    int arguments_count = -1;
    char *arguments = NULL;
};

enum class OPCODES
{
    HALT,
    PUSH,
    POP,
    ADD,
    SUB,
    MUL,
    DIV,
    OUT,
    IN,
    TOTAL_COMMANDS
};

enum class ARG_COUNT
{
    ZERO,
    ONE,
    TOTAL_ARGS
};

const command_t commands_table[(long long unsigned int) OPCODES::TOTAL_COMMANDS] = {
    {"halt", (int) OPCODES::HALT, (int) ARG_COUNT::ZERO},
    {"push", (int) OPCODES::PUSH, (int) ARG_COUNT::ONE},
    {"pop", (int) OPCODES::POP, (int) ARG_COUNT::ZERO},
    {"add", (int) OPCODES::ADD, (int) ARG_COUNT::ZERO},
    {"sub", (int) OPCODES::SUB, (int) ARG_COUNT::ZERO},
    {"mul", (int) OPCODES::MUL, (int) ARG_COUNT::ZERO},
    {"div", (int) OPCODES::DIV, (int) ARG_COUNT::ZERO},
    {"out", (int) OPCODES::OUT, (int) ARG_COUNT::ZERO},
    {"in", (int) OPCODES::IN, (int) ARG_COUNT::ZERO},
};


#endif  // COMMANDS_H
