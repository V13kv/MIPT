#ifndef LINES_SORT_H
#define LINES_SORT_H

#include <stdbool.h>

#include "../include/text.h"

typedef enum CMP_RESULTS
{
    CMP_FIRST_IS_LOWER = -1,
    CMP_ARE_EQUAL,
    CMP_FIRST_IS_GREATER
} cmp_results_et;

typedef enum CMP_OPTIONS
{
    CMP_OPTIONS_REVERSED = -1,
    CMP_OPTIONS_DIRECT = 1
} cmp_options_et;

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
void skip_letters(char **str_p, cmp_options_et cmp_option);

/**
 * @brief Compare lines excluding punctuation
 * 
 * @param a 
 * @param b 
 * @return int 
 */
int directLinesComparison(const void* a, const void* b);

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
void swap(text_line_t *line1, text_line_t *line2);

/**
 * @brief Quick sort wrapper. Checks for errors then call actual quick sort
 * 
 * @param text 
 * @param comp 
 */
void my_qsort(text_t *text, int (*comp)(const void *, const void *));

/**
 * @brief Bubble sort to test comparator and swap functions
 * 
 * @param text 
 * @param comp 
 */
void bubbleSort(text_t *text, int (*comp)(void const *, void const*));


#endif  // LINES_SORT_H
