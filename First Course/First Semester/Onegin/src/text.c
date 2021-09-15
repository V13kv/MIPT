#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

#include "../include/text.h"


text_st getTextObject(FILE *fs)
{
    // Get text size
    const int capacity = getTextSize(fs);

    // Get the text data as null terminated string
    char *data = (char *) malloc(sizeof(char) * (capacity + 1));
    fread(data, sizeof(char), capacity, fs);
    data[capacity] = '\0';

    // Get rid of empty lines
    deleteEmptyLines(data);

    // Get total amount of non-empty lines
    const int lines_count = getTotalAmountOfLines(data);

    // Create a text structure (object)
    text_st text = {data, capacity, lines_count};

    return text;
}

void freeTextObject(text_st text)
{
    free(text.data);
}

int getTextSize(FILE *fs)
{
    fseek(fs, 0, SEEK_END);
    int fsize = ftell(fs);
    fseek(fs, 0, SEEK_SET);

    return fsize;
}

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

    if (*(write - 1) != '\n')
    {
        *write++ = '\n';
        *write = '\0';
    }
    else
    {
        *write = '\0';
    }
}

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

void printTextObject(text_st text)
{
    printf("text.capacity: %d\n", text.capacity);
    printf("text.lines_count: %d\n", text.lines_count);

    for (int i = 0; i < text.capacity; ++i)
    {
        putchar(text.data[i]);
    }
    putchar('\n');
}

text_lines_st* getTextLinesObject(text_st text)
{
    // Allocate memory for each line of a text
    text_lines_st *lines = malloc(sizeof(text_lines_st) * text.lines_count);

    // Fill each line structure of the lines array
    char *text_p = text.data;
    for (int line = 0; line < text.lines_count; ++line)
    {
        lines[line].beginning = text_p;

        int line_length = 0;
        while (*text_p++ != '\n')
        {
            ++line_length;
        }
        ++line_length;  // Counting \n symbol as a part of a string

        lines[line].length = line_length;
    }

    return lines;
}

void freeTextLinesObject(text_lines_st *lines)
{
    free(lines);
}

void saveTextLinesObject(text_lines_st *lines, int lines_count, FILE *fs)
{
    for (int line = 0; line < lines_count; ++line)
    {
        fwrite(lines[line].beginning, sizeof(char), lines[line].length, fs);
        ++line;
    }
}

void printSeveralTextLines(text_lines_st *lines, int lines_to_print, int total_lines)
{
    assert(lines_to_print < total_lines && "[!] You are trying to print lines more than you have!");

    for (int line = 0; line < lines_to_print; ++line)
    {
        printf("#%d line: ", line + 1);

        int printed_characters = 0;
        while (printed_characters < lines[line].length)
        {
            printf("%c", lines[line].beginning[printed_characters]);
            ++printed_characters;
        }

        printf("length = %d\n", lines[line].length);
    }
}

cmp_res_et lexicographicComparison(text_lines_st a, text_lines_st b)
{
    char *a_p = a.beginning;
    char *b_p = b.beginning;

    while (*a_p != '\n' && *b_p != '\n')
    {
        if (*a_p < *b_p)
        {
            return SECOND_LINE_IS_GREATER;
        }
        else if (*a_p > *b_p)
        {
            return SECOND_LINE_IS_LOWER;
        }

        ++a_p;
        ++b_p;
    }

    if (*a_p == '\n')
    {
        --a_p;
    }
    if (*b_p == '\n')
    {
        --b_p;
    }  

    if (*a_p < *b_p)
    {
        return SECOND_LINE_IS_GREATER;
    }
    else if (*a_p > *b_p)
    {
        return SECOND_LINE_IS_LOWER;
    }
    else
    {
        return LINES_ARE_EQUAL;
    }
}

char* my_strdup(char *line_data, int line_length)
{
    char *temp = malloc(sizeof(char) * line_length);
    char *temp_p = temp;
    char *line_data_p = line_data;

    while (*line_data_p != '\n')
    {
        *temp = *line_data_p;

        ++temp_p;
        ++line_data_p;
    }
    *temp_p = '\n';

    return temp;
}

void swap(text_lines_st *line1, text_lines_st *line2)
{
    text_lines_st temp = *line1;
    *line1 = *line2;
    *line2 = temp;
}

void lexicographicTextLinesSort(text_lines_st *lines, const int lines_count)
{
    for (int first_line = 0; first_line < lines_count; ++first_line)
    {
        for (int second_line = first_line + 1; second_line < lines_count; ++second_line)
        {
            cmp_res_et result = lexicographicComparison(lines[first_line], lines[second_line]);
            switch (result)
            {
                case SECOND_LINE_IS_LOWER:
                    swap(&lines[first_line], &lines[second_line]);
                    break;
                case SECOND_LINE_IS_GREATER:
                    break;
                case LINES_ARE_EQUAL:
                    break;
            }
        }
    }
}