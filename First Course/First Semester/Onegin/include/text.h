#ifndef TEXT_H
#define TEXT_H

#include <stdio.h>
#include "../include/constants.h"

text_st getTextObject(FILE *fs);
void freeTextObject(text_st text);

int getTextCapacity(FILE *fs);
void deleteEmptyLines(char *text);
void convertLinesToCStrings(char *text);
int getTotalAmountOfLines(char *text_data);
void printTextObject(text_st text, int symbols_to_print);

#endif  // TEXT_H