#ifndef TEXT_H
#define TEXT_H

#include <stdio.h>

enum class FILE_MODE
{
    R,
    RB,
    A,
    W
};

/**
 * @brief Structure represents each line of the text in a file
 * 
 */
typedef struct
{
    char *beginning;
    size_t length;
} text_line_t;

/**
 * @brief Structure represents the text in a file
 * 
 */
typedef struct 
{
    char *data;
    size_t size;
    int lines_count;
    text_line_t *lines;
} text_t;  // TODO: buffer_st, text_t, text_line_t embedded structures (one structure)

/**
 * @brief Get a text structure representing the text of the file with the calculated values (see text_t structure)
 * 
 * @param text 
 * @param file_path 
 */
void textCtor(text_t *text, const char *file_path, const FILE_MODE mode);

char *getStrMode(const FILE_MODE mode);

/**
 * @brief Remove blank lines from a buffer
 * 
 * @param text 
 */
void deleteEmptyLines(char *text);

/**
 * @brief Strip line
 * 
 * @param line 
 * @return char* 
 */
char *stripLine(char *line);

/**
 * @brief Converts lines ending in '\n' to C-strings ending in '\0'
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
void printTextObject(text_t *text, size_t symbols_to_print);

/**
 * @brief Free the memory allocated for the text of the file
 * 
 * @param text 
 */
void textDtor(text_t *text);

/**
 * @brief Extract lines from text using lines structure
 * 
 * @param text 
 * @return text_line_t* 
 */
text_line_t* getTextLines(text_t *text);

/**
 * @brief Writing line objects to file
 * 
 * @param text 
 * @param file_name 
 * @param mode 
 */
void exportTextObject(const text_t *const text, const char *file_name, const FILE_MODE mode);

/**
 * @brief Printing 'lines_to_print' lines from the beginning of the text
 * 
 * @param text 
 * @param lines_to_print 
 */
void printTextLines(const text_t *const text, const int lines_to_print);


#endif  // TEXT_H
