#ifndef LINES_H
#define LINES_H


#include <stdio.h>
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

//TODO: optimize
int normalComparison(const void* a, const void* b);
int reversedComparison(const void *a, const void *b);

//TODO: void my_qsort(text_lines_st *lines, int low, int high, int (*comp)(const void *, const void *));
///TODO: void swap(text_lines_st *line1, text_lines_st *line2);


/*
void lexicographicTextLinesSort(text_lines_st *lines, const int lines_count);
cmp_res_et lexicographicComparison(text_lines_st a, text_lines_st b);
void swap(text_lines_st *line1, text_lines_st *line);
*/


#endif  // LINES_H