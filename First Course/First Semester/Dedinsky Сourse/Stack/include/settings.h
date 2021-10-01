#ifndef STACK_SETTINGS_H
#define STACK_SETTINGS_H


#pragma region STACK_INTERNALS

    #define stackElem_t                 int     // Stack is consists of stackElem_t type variables only
    #define stackReallocCoefficient     2       // Determines how many times the stack increases/decreases
    #define POISON                      -663    // Poison for identifying unused values

#pragma endregion STACK_INTERNALS

#pragma region STACK_SECURITY

    //#define STACK_CANARY        0  // Enable canary
    #define STACK_CANARY        1  // Disable canary

    #if defined(STACK_CANARY) && STACK_CANARY == 1
        #define LEFT_CANARY_SIZE    sizeof(stackElem_t)
        #define RIGHT_CANARY_SIZE   sizeof(stackElem_t)
        #define CANARY_VALUE        0xBABE
    #endif

    //#define STACK_HASH      0  // Enable hash sum
    #define STACK_HASH      1  // Disable hash sum

#pragma endregion STACK_SECURITY


#endif  // STACK_SETTINGS_H