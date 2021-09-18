#ifndef LINES_H
#define LINES_H


#define CMP_LINES_WO_PUNCTUATION  // Used to exclude punctuation from lines comparison functions (comment/uncomment)

#include <stdio.h>
#include <stdbool.h>

#include "../include/text.h"


typedef enum CMP_RESULTS
{
    CMP_FIRST_IS_LOWER = -1,
    CMP_ARE_EQUAL,
    CMP_FIRST_IS_GREATER
} cmp_results_et;

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
text_line_st* get_text_lines(text_st *text);

/**
 * @brief Writing line objects to file
 * 
 * @param lines 
 * @param count_lines 
 * @param file_name 
 */
void export_text_line_objects(const text_line_st *const lines, const int lines_count, const char *file_name);

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
 * @brief Passing punctuation
 * 
 * @param str_p 
 */
void skip_letters(char **str_p);

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
void my_strrev(char *const str, const int len);

//FIXME: rewrite function without 4 strrev's
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
int _partition(text_line_st *const lines, int low, int high, int (*comp)(const void*, const void *));

/**
 * @brief Quick sort algorithm implemented to sort line structures
 * 
 * @param lines 
 * @param low 
 * @param high 
 * @param comp 
 */
void _my_qsort(text_line_st *lines, int low, int high, int (*comp)(const void *, const void *));

/**
 * @brief Quick sort wrapper. Checks for errors then call actual quick sort
 * 
 * @param lines 
 * @param low 
 * @param high 
 * @param comp 
 */
void my_qsort(text_line_st *lines, int low, int high, int (*comp)(const void *, const void *));

/**
 * @brief Bubble sort to test comparator and swap functions
 * 
 * @param lines 
 * @param lines_count 
 * @param comp 
 */
void bubbleSort(text_line_st *lines, int lines_count, int (*comp)(void const *, void const*));


#endif  // LINES_H