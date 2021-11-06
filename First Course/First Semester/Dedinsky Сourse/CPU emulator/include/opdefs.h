#ifndef REDEFINE_VALUES
    #define VAL     val
    #define VAL_1   val1
    #define VAL_2   val2
    #define WINDOW  window
#endif

#define GET_VALUE() cpuGetBytecodeValue(CPU, byteCode)

#define PUSH(value)         cpuPush(CPU, value)
#define POP()               cpuPop(CPU)
#define MOVE_VALUE(value)   cpuMoveValue(CPU, byteCode, value)
#define JUMP()              cpuJump(CPU, byteCode)
#define JE()                cpuJE(CPU, byteCode)
#define JNE()               cpuJNE(CPU, byteCode)
#define JL()                cpuJL(CPU, byteCode)
#define JG()                cpuJG(CPU, byteCode)
#define CALL()              cpuCall(CPU, byteCode)
#define RET()               cpuRet(CPU)
#define CMP()               cpuCmp(CPU)
#define OUT()               cpuOut(CPU)
#define OUTC()              cpuOutc(CPU)
#define IN()                cpuIn(CPU)
#define EXIT(exitCode)      cpuExit(CPU, byteCode, exitCode)

// TODO: gpu commands

// opMnemonics, opcode, argc, code
OPDEF(push, 0, 1, {
    VAL = GET_VALUE();
    PUSH(VAL);
})

OPDEF(pop, 1, 1, {
    VAL = POP(); // TODO: support of pop ax; pop [ax]. Error check pop immediate (it is not possible)
    MOVE_VALUE(VAL);
})

OPDEF(add, 2, 0, {
    VAL_1 = POP();
    VAL_2 = POP();
    PUSH(VAL_1 + VAL_2);
})

OPDEF(sub, 3, 0, {
    VAL_1 = POP();
    VAL_2 = POP();
    PUSH(VAL_1 - VAL_2);
})

OPDEF(mul, 4, 0, {
    VAL_1 = POP();
    VAL_2 = POP();
    PUSH(VAL_1 * VAL_2);
})

OPDEF(div, 5, 0, {
    VAL_1 = POP();
    VAL_2 = POP();
    PUSH(VAL_1 / VAL_2);
})

OPDEF(out, 6, 0, {
    OUT();
})

OPDEF(in, 7, 0, {
    IN();
})

OPDEF(jmp, 8, 1, {
    JUMP();
})

OPDEF(call, 9, 1, {
    CALL();
})

OPDEF(ret, 10, 0, {
    RET();
})

OPDEF(outc, 11, 0, {
    OUTC();
})

OPDEF(sqrt, 12, 0, {
    VAL = sqrt(POP());
    PUSH(VAL);
})

OPDEF(je, 13, 1, {
    JE();
})

OPDEF(jl, 14, 1, {
    JL();
})

OPDEF(cmp, 15, 0, {
    CMP();
})

OPDEF(jg, 16, 1, {
    JG();
})

OPDEF(jne, 17, 1, {
    JNE();
})

OPDEF(halt, 255, 0, {
    EXIT(EXIT_SUCCESS);
})

// -------------------------------------------------GPU COMMANDS-------------------------------------------------

OPDEF(ginit, 18, 0, {
    window = sfRenderWindow_create({WIDTH, HEIGHT, BITS_PER_PIXEL}, WINDOW_NAME, sfResize | sfClose, &settings);
})

// --------------------------------------------------------------------------------------------------------------
