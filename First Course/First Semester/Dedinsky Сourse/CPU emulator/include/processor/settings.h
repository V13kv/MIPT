#ifndef PROCESSOR_SETTINGS_H
#define PROCESSOR_SETTINGS_H


#define MIN_RAM_SIZE                200      
#define DEFAULT_DOUBLE_VALUE        0
#define BAD_DOUBLE_VALUE            -663

#define MRI_IS_IMMEDIATE(byteCodeByte)  (byteCodeByte & 0b001) != 0
#define MRI_IS_REGISTER(byteCodeByte)   (byteCodeByte & 0b010) != 0
#define MRI_IS_MEMORY(byteCodeByte)     (byteCodeByte & 0b100) != 0
#define GET_TOTAL_ARGS(byteCodeByte)    (byteCodeByte & 0b11100000) >> 5
#define GET_GLOBAL_MRI(byteCodeByte)    (byteCodeByte & 0b00011100) >> 2

const int RAM_CELLS_TO_DUMP            = 15;
const double EPS                       = 0.001;
const double DOUBLES_ARE_EQUAL         = 0;
const double FIRST_DOUBLE_IS_GREATER   = 1;
const double FIRST_DOUBLE_IS_LOWER     = -1;


#endif  // PROCESSOR_SETTINGS_H
