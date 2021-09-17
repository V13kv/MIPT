#ifndef LINES_H
#define LINES_H


#include <stdio.h>
#include <stdbool.h>

#include "../include/constants.h"

typedef struct
{
    char *beginning;
    int length;
} text_lines_st;

text_lines_st* getTextLinesObject(text_st text);
void saveTextLinesObject(text_lines_st *lines, int count_lines, FILE *fs);
void freeTextLinesObject(text_lines_st *lines);
void printSeveralTextLines(text_lines_st *lines, int lines_to_print, int total_lines);


bool isLetter(char chr);
int directLinesComparison(const void* a, const void* b);
void my_strrev(char *str, int len);
int reversedLinesComparison(const void *a, const void *b);

void swap(text_lines_st *line1, text_lines_st *line2);
//TODO: void my_qsort(text_lines_st *lines, int low, int high, int (*comp)(const void *, const void *));


#endif  // LINES_H