#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "../include/text.h"


text_st getTextObject(FILE *fs)
{
    assert(fs != NULL && "[!] You have passed a null pointer as a file stream!");

    // Get file capacity
    const int file_capacity = getTextCapacity(fs);

    // Get the text data as null-terminated string AND get the size of the text
    char *data = (char *) malloc(sizeof(char) * (file_capacity + 1));
    assert(data != NULL && "[!] Got a null pointer after malloc function!");

    int size = fread(data, sizeof(char), file_capacity, fs);
    assert(ferror(fs) == 0 && "[!] There was an error during reading the file stream!");
    data[size] = '\0';

    // Get rid of empty lines
    deleteEmptyLines(data);

    // Get text size
    size = strlen(data);
    assert(size != 0 && "[!] Got a null pointer after strlen function!");

    // Get total amount of non-empty lines
    const int lines_count = getTotalAmountOfLines(data);

    // Change the \n character to \0 to interact with lines as null-terminated string
    convertLinesToCStrings(data);

    // Create a text structure
    text_st text = {data, size, lines_count};

    return text;
}

int getTextCapacity(FILE *fs)
{
    assert(fs != NULL && "[!] You have passed a null pointer as a file_stream!");

    fseek(fs, 0, SEEK_END);
    int fsize = ftell(fs);
    fseek(fs, 0, SEEK_SET);

    return fsize;
}

void deleteEmptyLines(char *text)
{
    assert(text != NULL && "[!] You have passed a null pointer as a file stream!");

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

void convertLinesToCStrings(char *text)
{
    assert(text != NULL && "[!] You have passed a null pointer as a file stream!");

    while (*text != '\0')
    {
        if (*text == '\n')
        {
            *text = '\0';
        }
        ++text;
    }
}

int getTotalAmountOfLines(char *text)
{
    assert(text != NULL && "[!] You have passed a null pointer as a file stream!");

    int lines = 0;
    while (*text != '\0')
    {
        if (*text == '\n')
        {
            ++lines;
        }
        ++text;     
    }

    if (*(text - 1) != '\n')
    {
        ++lines;
    }

    return lines;
}

void printTextObject(text_st *text, int symbols_to_print)
{
    assert(symbols_to_print <= text->size && "[!] You are trying to print text symbols more than you have!");

    printf("text->size: %d\n", text->size);
    printf("text->lines_count: %d\n", text->lines_count);

    for (int symbol = 0; symbol < symbols_to_print; ++symbol)
    {
        if (text->data[symbol] == '\0')
        {
            putchar('\n');
        }
        else
        {
            putchar(text->data[symbol]);
        }
    }
    putchar('\n');
}

void freeTextObject(text_st text)
{
    free(text.data);
}