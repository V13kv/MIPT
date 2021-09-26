#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "../include/text.h"


void text_st_constructor(text_st *text, const char *file_path)
{
    assert(text != NULL && "[!] You have passed a null pointer as a text_st structure!");
    assert(file_path != NULL && "[!] You have passed a null pointer as a file_path parameter!");

    // Open file
    FILE *fs = fopen(file_path, "r");
    assert(fs != NULL && "[!] Got a null pointer after fopen function!");

    // Get file capacity
    const int file_capacity = get_file_capacity(fs);
    
    // Get the text data as null-terminated string AND get the size of the text
    char *data = (char *) malloc(sizeof(char) * (file_capacity + 1));
    assert(data != NULL && "[!] Got a null pointer after malloc function!");

    size_t size = fread(data, sizeof(char), file_capacity, fs);
    assert(ferror(fs) == 0 && "[!] There was an error during reading the file stream!");
    data[size] = '\0';

    // Get rid of empty lines
    delete_empty_lines(data);

    // Get text size
    size = strlen(data);
    assert(size != 0 && "[!] Got a null pointer after strlen function!");

    // Get total amount of non-empty lines
    const int lines_count = get_total_amount_of_lines(data);

    // Change the \n character to \0 to interact with lines as null-terminated string
    convert_lines_to_CStrings(data);

    // Fill text structure
    text->data = data;
    text->size = size;
    text->lines_count = lines_count;
    
    // Free file stream object
    fclose(fs);
}

int get_file_capacity(FILE *fs)
{
    assert(fs != NULL && "[!] You have passed a null pointer as a file_stream!");

    fseek(fs, 0, SEEK_END);
    int fsize = ftell(fs);
    fseek(fs, 0, SEEK_SET);

    return fsize;
}

void delete_empty_lines(char *text)
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
}

void convert_lines_to_CStrings(char *text)
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

int get_total_amount_of_lines(char *text)
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

void text_st_print(text_st *text, size_t symbols_to_print)
{
    assert(text != NULL && "[!] You have passed a null pointer as a text_st structure!");
    assert(text->data != NULL && "[!] text_st structure has text.data as a null pointer!");
    assert(symbols_to_print <= text->size && "[!] You are trying to print text symbols more than you have!");

    printf("text->size: %zu\n", text->size);
    printf("text->lines_count: %d\n", text->lines_count);

    for (size_t symbol = 0; symbol < symbols_to_print; ++symbol)
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

void text_st_deconstructor(text_st *text)
{
    free(text->data);
}
