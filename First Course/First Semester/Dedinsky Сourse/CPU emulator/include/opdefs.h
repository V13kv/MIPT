// #ifndef OPDEFS_H
// #define OPDEFS_H

// #define PUSH
// #define GET_VALUE
// #define VAL_1

// opMnemonics, opcode, opArgsCount, code
OPDEF(push, 0, 1, {})

OPDEF(pop, 1, 0, {})

OPDEF(add, 2, 0, {})

OPDEF(sub, 3, 0, {})

OPDEF(mul, 4, 0, {})

OPDEF(out, 6, 0, {})

OPDEF(div, 5, 0, {})

OPDEF(in, 7, 0, {})

OPDEF(jmp, 8, 1, {})

OPDEF(call, 9, 1, {})

OPDEF(halt, 255, 0, {})

// #endif  // OPDEFS_H
