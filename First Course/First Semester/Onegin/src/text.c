#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "../include/text.h"


/**
 * @brief Get a text structure representing the text of the file with the calculated values (see text_st structure)
 * 
 * @param fs 
 * @return text_st 
 */
text_st getTextObject(FILE *fs)
{
    // Get file capacity
    const int file_capacity = getTextCapacity(fs);

    // Get the text data as null-terminated string AND get the size of the text
    char *data = (char *) malloc(sizeof(char) * (file_capacity + 1));
    int size = fread(data, sizeof(char), file_capacity, fs);
    data[size] = '\0';

    // Get rid of empty lines
    deleteEmptyLines(data);

    // Get text size
    size = strlen(data);

    // Get total amount of non-empty lines
    const int lines_count = getTotalAmountOfLines(data);

    // Change the \n character to \0 to interact with lines as null-terminated string
    convertLinesToCStrings(data);

    // Create a text structure
    text_st text = {data, size, lines_count};

    return text;
}

/**
 * @brief Get the capacity of the text in the file
 * 
 * @param fs 
 * @return int 
 */
int getTextCapacity(FILE *fs)
{
    fseek(fs, 0, SEEK_END);
    int fsize = ftell(fs);
    fseek(fs, 0, SEEK_SET);

    return fsize;
}

/**
 * @brief Remove blank lines from a buffer
 * 
 * @param text 
 */
void deleteEmptyLines(char *text)
{
    char *read = text;
    char *write = text;
    char previous_char = ' ';

    while (*read != '\0')
    {
        if (*read != '\n' || previous_char != '\n')
        {
            *write = *read;
            ++write;
        }

        previous_char = *read;
        ++read;
    }
    *write = '\0';

    /*
    if (*--write != '\n')
    {
        *write++ = '\n';
        *write = '\0';
    }
    else
    {
        *write = '\0';
    }
    */
}

/**
 * @brief Converts lines ending in '\n' to C-string ending in '\0'
 * 
 * @param text 
 */
void convertLinesToCStrings(char *text)
{
    while (*text != '\0')
    {
        if (*text == '\n')
        {
            *text = '\0';
        }
        ++text;
    }
}

/**
 * @brief Get the total amount of lines in a buffer
 * 
 * @param text 
 * @return int 
 */
int getTotalAmountOfLines(char *text)
{
    int lines = 0;
    while (*text != '\0')
    {
        if (*text == '\n')
        {
            ++lines;
        }
        ++text;     
    }

    return lines;
}

/**
 * @brief Print basic information about the structure including first 'symbols_to_print' symbols
 * 
 * @param text 
 * @param symbols_to_print 
 */
void printTextObject(text_st text, int symbols_to_print)
{
    assert(symbols_to_print <= text.size && "[!] You are trying to print text symbols more than you have!");

    printf("text.size: %d\n", text.size);
    printf("text.lines_count: %d\n", text.lines_count);

    for (int symbol = 0; symbol < symbols_to_print; ++symbol)
    {
        if (text.data[symbol] == '\0')
        {
            putchar('\n');
        }
        else
        {
            putchar(text.data[symbol]);
        }
    }
    putchar('\n');
}

/**
 * @brief Free the memory allocated for the text of the file
 * 
 * @param text 
 */
void freeTextObject(text_st text)
{
    free(text.data);
}