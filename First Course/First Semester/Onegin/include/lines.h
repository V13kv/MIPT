#ifndef LINES_H
#define LINES_H


#define CMP_LINES_WO_PUNCTUATION  // Used to exclude punctuation from lines comparison functions (comment/uncomment)
#define FIRST_LINE_IS_LOWER -1  // my_qsort comparator a < b result (in human readable format)

#include <stdio.h>
#include <stdbool.h>

#include "../include/constants.h"

/**
 * @brief Structure represents each line of the text in a file
 * 
 */
typedef struct
{
    char *beginning;
    int length;
} text_line_st;

/**
 * @brief Extract lines from text using lines structure
 * 
 * @param text 
 * @return text_line_st* 
 */
text_line_st* getTextLinesObject(text_st text);

/**
 * @brief Writing line objects to file
 * 
 * @param lines 
 * @param count_lines 
 * @param fs 
 */
void saveTextLinesObject(const text_line_st *const lines, const int count_lines, FILE *fs);

/**
 * @brief Free the memory allocated for the lines of the file
 * 
 * @param lines 
 */
void freeTextLinesObject(text_line_st *lines);


/**
 * @brief Printing 'lines_to_print' lines from the beginning of the text
 * 
 * @param lines 
 * @param lines_to_print 
 * @param total_lines 
 */
void printSeveralTextLines(const text_line_st *const lines, const int lines_to_print, const int total_lines);


/**
 * @brief Determine whether the passed char is not a punctuation symbol
 * 
 * @param chr 
 * @return true 
 * @return false 
 */
bool isLetter(const char chr);

/**
 * @brief Compare lines excluding punctuation
 * 
 * @param a 
 * @param b 
 * @return int 
 */
int directLinesComparison(const void* a, const void* b);

/**
 * @brief String line reverse
 * 
 * @param str 
 * @param len 
 */
void my_strrev(char *str, const int len);

/**
 * @brief Wrapper for lines camparison in reversed order
 * 
 * @param a 
 * @param b 
 * @return int 
 */
int reversedLinesComparison(const void *a, const void *b);


/**
 * @brief Swapping two lines structures
 * 
 * @param line1 
 * @param line2 
 */
void swap(text_line_st *line1, text_line_st *line2);

/**
 * @brief Part of a my_qsort (quick sort) algorithm
 * 
 * @param lines 
 * @param low 
 * @param high 
 * @param comp 
 * @return int 
 */
int partition(text_line_st *lines, const int low, const int high, int (*comp)(const void*, const void *));

/**
 * @brief Quick sort algorithm implemented to sort lines structures
 * 
 * @param lines 
 * @param low 
 * @param high 
 * @param comp 
 */
void my_qsort(text_line_st *lines, const int low, const int high, int (*comp)(const void *, const void *));


#endif  // LINES_H