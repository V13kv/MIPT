#ifndef TEXT_H
#define TEXT_H

#include <stdio.h>
#include "../include/constants.h"

/**
 * @brief Get a text structure representing the text of the file with the calculated values (see text_st structure)
 * 
 * @param fs 
 * @return text_st 
 */
text_st getTextObject(FILE *fs);

/**
 * @brief Get the capacity of the text in the file
 * 
 * @param fs 
 * @return int 
 */
int getTextCapacity(FILE *fs);

/**
 * @brief Remove blank lines from a buffer
 * 
 * @param text 
 */
void deleteEmptyLines(char *text);

/**
 * @brief Converts lines ending in '\n' to C-string ending in '\0'
 * 
 * @param text 
 */
void convertLinesToCStrings(char *text);

/**
 * @brief Get the total amount of lines in a buffer
 * 
 * @param text 
 * @return int 
 */
int getTotalAmountOfLines(char *text_data);

/**
 * @brief Print basic information about the structure including first 'symbols_to_print' symbols
 * 
 * @param text 
 * @param symbols_to_print 
 */
void printTextObject(text_st *text, int symbols_to_print);

/**
 * @brief Free the memory allocated for the text of the file
 * 
 * @param text 
 */
void freeTextObject(text_st text);


#endif  // TEXT_H