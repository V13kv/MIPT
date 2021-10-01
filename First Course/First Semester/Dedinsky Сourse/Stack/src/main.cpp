#include <stdio.h>
#include <stdlib.h>

#include "../include/stack.h"

//TODO: переделать hash сумму под байты (считать по байтам в цикле, чтобы не зависеть от структуры (при добавлении нового поля в структуре))
//TODO: переделать канарейки под байты (также как и выше)
//TODO: переименовать stackCapacitySecurityIncrease в более подходящее
//TODO: расставить проверки везде где только возможно
//TODO: сделать логичным кол-во функций hashSum (посмотреть)
int main(void)
{
    stack_t stack = {};
    IS_OK_W_EXIT(stackCtor(&stack, 8));

    // -----------------------------------Hash sum and canary check-----------------------------------
    // stack.data = (int *) malloc(sizeof(int));
    // IS_OK_W_EXIT(stackPop(&stack));

    // -----------------------------------Standard debug macroses test-----------------------------------
    // for (int i = 0; i < 2; ++i)
    // {
    //     IS_OK_W_EXIT(stackPush(&stack, 123));
    // }

    // for (int i = 0; i < 3; ++i)
    // {
    //     IS_OK_W_EXIT(stackPop(&stack));
    // }

    // -----------------------------------Canary test-----------------------------------
    // *((int*)(&stack)) = -1;
    // IS_OK_W_EXIT(stackReallocation(&stack, REALLOC_MODES::INCREASE));

    stackDtor(&stack);

    return 0;
}
