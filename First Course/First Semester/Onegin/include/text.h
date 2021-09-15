#ifndef TEXT_H
#define TEXT_H

#include <stdio.h>

typedef struct 
{
    char *const data;
    const int capacity;
    const int lines_count;
} text_st;

text_st getTextObject(FILE *fs);
void freeTextObject(text_st text);

int getTextSize(FILE *fs);
int getTotalAmountOfLines(char *text_data);
void deleteEmptyLines(char *text);
void printTextObject(text_st text);

typedef struct TEXT_LINES
{
    char *beginning;
    int length;
} text_lines_st;

typedef enum LINE_COMPARISON_RESULTS
{
    SECOND_LINE_IS_LOWER,
    SECOND_LINE_IS_GREATER,
    LINES_ARE_EQUAL
} cmp_res_et;

text_lines_st* getTextLinesObject(text_st text);
void freeTextLinesObject(text_lines_st *lines);
void saveTextLinesObject(text_lines_st *lines, int count_lines, FILE *fs);

void printSeveralTextLines(text_lines_st *lines, int lines_to_print, int total_lines);
void lexicographicTextLinesSort(text_lines_st *lines, const int lines_count);
cmp_res_et lexicographicComparison(text_lines_st a, text_lines_st b);
void swap(text_lines_st *line1, text_lines_st *line);


#endif  // TEXT_H