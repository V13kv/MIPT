#ifndef TEXT_CONSTANTS_H
#define TEXT_CONSTANTS_H


/**
 * @brief Structure represents the text in a file
 * 
 */
typedef struct 
{
    char *const data;
    int size;
    const int lines_count;
} text_st;


#endif  // TEXT_CONSTANTS_H