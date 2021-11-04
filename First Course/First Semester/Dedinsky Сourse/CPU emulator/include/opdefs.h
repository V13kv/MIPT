#ifndef REDEFINE_VALUES
    #define VAL     val
    #define VAL_1   val1
    #define VAL_2   val2
#endif

#define GET_VALUE() cpuGetBytecodeValue(CPU, byteCode)

#define PUSH(value)     cpuPush(CPU, value)
#define POP()           cpuPop(CPU)
#define JUMP()          cpuJump(CPU, byteCode)
#define CALL()          cpuCall(CPU, byteCode)
#define RET()           cpuRet(CPU)
#define OUT()           cpuOut(CPU)
#define OUTC()          cpuOutc(CPU)
#define IN()            cpuIn(CPU)
#define EXIT(exitCode)  cpuExit(CPU, byteCode, exitCode)

// TODO: condition jumps, gpu commands

// opMnemonics, opcode, argc, code
OPDEF(push, 0, 1, {
    VAL = GET_VALUE();  // TODO: support of memory, regs, immediate as GET_VALUE()
    PUSH(VAL);
}) // +

OPDEF(pop, 1, 0, {
    VAL = POP();
}) // +

OPDEF(add, 2, 0, {
    VAL_1 = POP();
    VAL_2 = POP();
    PUSH(VAL_1 + VAL_2);
}) // +

OPDEF(sub, 3, 0, {
    VAL_1 = POP();
    VAL_2 = POP();
    PUSH(VAL_1 - VAL_2);
}) // +

OPDEF(mul, 4, 0, {
    VAL_1 = POP();
    VAL_2 = POP();
    PUSH(VAL_1 * VAL_2);
}) // +

OPDEF(out, 6, 0, {
    OUT();
}) // +

OPDEF(outc, 11, 0, {
    OUTC();
}) // +

OPDEF(div, 5, 0, {
    VAL_1 = POP();
    VAL_2 = POP();
    PUSH(VAL_1 / VAL_2);
}) // +

OPDEF(in, 7, 0, {
    IN();
}) // +

OPDEF(jmp, 8, 1, {
    JUMP();
}) // +

OPDEF(call, 9, 1, {
    CALL();
}) // +

OPDEF(ret, 10, 0, {
    RET();
}) // +

OPDEF(halt, 255, 0, {
    EXIT(EXIT_SUCCESS);
}) // +
