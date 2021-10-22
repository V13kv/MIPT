#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "../include/file.h"
#include "../include/text.h"

char *getStrMode(const FILE_MODE mode)
{
    char *strMode = NULL;
    switch (mode)
    {
        case FILE_MODE::R:
            strMode = "r";
            break;
        case FILE_MODE::RB:
            strMode = "rb";
            break;
        case FILE_MODE::A:
            strMode = "a";
            break;
        case FILE_MODE::W:
            strMode = "w";
            break;
        default:
            assert(0 && "Bad mode argument passed!");
            break;
    }

    return strMode;
}

void textCtor(text_t *text, const char *file_path, const FILE_MODE mode)
{
    assert(text != NULL && "[!] You have passed a null pointer as a text_t structure!");
    assert(file_path != NULL && "[!] You have passed a null pointer as a file_path parameter!");
    assert((mode == FILE_MODE::R || mode == FILE_MODE::RB) && "[!] You have passed bad FILE_MODE mode!");

    // Open file
    FILE *fs = fopen(file_path, getStrMode(mode));
    assert(fs != NULL && "[!] Got a null pointer after fopen function!");

    // Get file capacity
    const int file_capacity = get_file_capacity(fs);
    
    // Get the text data as null-terminated string AND get the size of the text
    char *data = (char *) calloc(file_capacity + 1, sizeof(char));
    assert(data != NULL && "[!] Got a null pointer after calloc function!");

    size_t size = fread(data, sizeof(char), file_capacity, fs);
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

    // Fill text structure
    text->data = data;
    text->size = size;
    text->lines_count = lines_count;

    // Get lines
    text->lines = getTextLines(text);
    
    // Free file stream object
    fclose(fs);
}

void textDtor(text_t *text)
{
    free(text->data);
    free(text->lines);
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

void printTextObject(text_t *text, size_t symbols_to_print)
{
    assert(text != NULL && "[!] You have passed a null pointer as a text_t structure!");
    assert(text->data != NULL && "[!] text_t structure has text.data as a null pointer!");
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

text_line_t* getTextLines(text_t *text)
{
    // Error check
    assert(text != NULL && "[!] You have passed a null pointer as a text_t structure!");

    // Allocate memory for all lines
    text_line_t *lines = (text_line_t *) calloc(text->lines_count, sizeof(text_line_t));
    assert(lines != NULL && "[!] Got a null pointer after calloc function!");

    // Constructing lines
    char *text_p = text->data;
    assert(text_p != NULL && "[!] You have passed a null pointer as a text.data!");

    //TODO: replace strlen with pointers subtraction
    for (int line = 0; line < text->lines_count; ++line)
    {
        lines[line].beginning = text_p;
        lines[line].length = strlen(text_p);

        text_p += lines[line].length + 1;
    }

    return lines;
}

void exportTextObject(const text_t *const text, const char *file_name, const FILE_MODE mode)
{
    // Create file_name file
    FILE *fs = fopen(file_name, getStrMode(mode));

    // Error check
    assert(fs != NULL && "[!] You have passed a null pointer as a file stream!");
    assert(text->lines != NULL && "[!] You have passed a null pointer as a lines array!");
    for (int line = 0; line < text->lines_count; ++line)
    {
        assert(text->lines[line].beginning != NULL && "[!] text_line_t structure has lines[ind].beginning as a null pointer");
    }
    assert(text->lines_count > 0 && "[!] You have passed negative lines_count parameter!");

    // Export
    for (int line = 0; line < text->lines_count; ++line)
    {
        fwrite(text->lines[line].beginning, sizeof(char), text->lines[line].length, fs);
        fputc('\n', fs);
    }

    // Cloes file stream object
    fclose(fs);
}

void printTextLines(const text_t *const text, const int lines_to_print)
{
    // Error check
    assert(text != NULL && "[!] You have passed a null pointer as a lines array!");
    for (int line = 0; line < lines_to_print; ++line)
    {
        assert(text->lines[line].beginning != NULL && "[!] text_line_t structure has lines[ind].beginning as a null pointer");
    }
    assert(lines_to_print <= text->lines_count && "[!] You are trying to print lines more than you have!");

    // Print lines
    putchar('\n');
    for (int line = 0; line < lines_to_print; ++line)
    {
        printf("#%d line: %s\n", line + 1, text->lines[line].beginning);
        printf("length = %zu\n\n", text->lines[line].length);
    }
}