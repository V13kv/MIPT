#ifndef TEXT_H
#define TEXT_H

#include <stdio.h>

/**
 * @brief Structure represents the text in a file
 * 
 */
typedef struct 
{
    char *data;
    int size;
    int lines_count;
} text_st;

/**
 * @brief Get a text structure representing the text of the file with the calculated values (see text_st structure)
 * 
 * @param text 
 * @param file_path 
 */
void text_st_constructor(text_st *text, const char *file_path);

/**
 * @brief Get the capacity of the text in the file
 * 
 * @param fs 
 * @return int 
 */
int get_text_capacity(FILE *fs);

/**
 * @brief Remove blank lines from a buffer
 * 
 * @param text 
 */
void delete_empty_lines(char *text);

/**
 * @brief Converts lines ending in '\n' to C-strings ending in '\0'
 * 
 * @param text 
 */
void convert_lines_to_CStrings(char *text);

/**
 * @brief Get the total amount of lines in a buffer
 * 
 * @param text 
 * @return int 
 */
int get_total_amount_of_lines(char *text_data);

/**
 * @brief Print basic information about the structure including first 'symbols_to_print' symbols
 * 
 * @param text 
 * @param symbols_to_print 
 */
void print_text_object(text_st *text, int symbols_to_print);

/**
 * @brief Free the memory allocated for the text of the file
 * 
 * @param text 
 */
void text_st_deconstructor(text_st *text);


#endif  // TEXT_H