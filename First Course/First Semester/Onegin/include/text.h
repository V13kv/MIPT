#ifndef TEXT_H
#define TEXT_H

#include <stdio.h>

typedef struct 
{
    char *const data;
    const int capacity;
    const int lines_count;
} text_t;

text_t getTextObject(FILE *fs);
void freeTextObject(text_t text);

int getTextSize(FILE *fs);
int getTotalAmountOfLines(char *text_data);
void deleteEmptyLines(char *text);
void printTextObject(text_t text);

//int getAmountOfNonEmptyLines(FILE *fs);


typedef struct 
{
    char *beginning;
    int length;
} text_lines_t;

text_lines_t* getTextLinesObject(text_t text);
void freeTextLinesObject(text_lines_t *lines);

void printSeveralTextLines(text_lines_t *lines, int lines_to_print);
void lexicographicTextLinesSort(text_lines_t *lines, const int lines_count);


#endif  // TEXT_H