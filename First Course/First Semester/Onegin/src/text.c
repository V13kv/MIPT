#include <stdio.h>
#include <stdlib.h>

#include "../include/text.h"


typedef enum LINE_COMPARISON_RESULTS
{
    SECOND_LINE_IS_LOWER,
    SECOND_LINE_IS_GREATER,
    LINES_ARE_EQUAL
} cmp_res_et;


text_t getTextObject(FILE *fs)
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
    text_t text = {data, capacity, lines_count};

    return text;
}

void freeTextObject(text_t text)
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

//TODO: connect function argument with text_t structure
void deleteEmptyLines(char *text)
{
    char *read = text;
    char *write = text;
    char previous_char = ' ';

    // it is assumed that there is a new line ('\n') at the end of the text
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

//TODO: connect function argument with text_t structure
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

void printTextObject(text_t text)
{
    printf("text.capacity: %d\n", text.capacity);
    printf("text.lines_count: %d\n", text.lines_count);
}

text_lines_t* getTextLinesObject(text_t text)
{
    // Allocate memory for each line of a text
    text_lines_t *lines = malloc(sizeof(text_lines_t) * text.lines_count);

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

        lines[line].length = line_length;
    }

    return lines;
}

void freeTextLinesObject(text_lines_t *lines)
{
    free(lines);
}

void printSeveralTextLines(text_lines_t *lines, int lines_to_print)
{
    //TODO: check whether the line_to_print variable is in text.lines_count range

    for (int line = 0; line < lines_to_print; ++line)
    {
        printf("#%d line: ", line + 1);

        int printed_characters = 0;
        while (printed_characters < lines[line].length)
        {
            printf("%c", lines[line].beginning[printed_characters]);
            ++printed_characters;
        }
        putchar('\n');

        printf("length = %d\n", lines[line].length);
    }
}

//FIXME:
cmp_res_et lexicographicComparison(text_lines_t a, text_lines_t b)
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

void lexicographicTextLinesSort(text_lines_t *lines, const int lines_count)
{
    //TODO: implementation
    for (int first_line = 0; first_line < lines_count; ++first_line)
    {
        for (int second_line = first_line + 1; second_line < lines_count; ++second_line)
        {
            cmp_res_et result = lexicographicComparison(lines[first_line], lines[second_line]);
            switch (result)
            {
                case SECOND_LINE_IS_LOWER:
                    //TODO: implementation
                    printf("Line %d > line %d\n", first_line+1, second_line+1);
                    break;
                case SECOND_LINE_IS_GREATER:
                    //TODO: implementation
                    printf("Line %d < line %d\n", first_line+1, second_line+1);
                    break;
                case LINES_ARE_EQUAL:
                    printf("Line %d == line %d\n", first_line+1, second_line+1);
                    //TODO: implementation
                    break;
            }
        }
    }
}


/*
int getAmountOfNonEmptyLines(FILE *fs)
{
    int     lines = 0;
    char    c = fgetc(fs);
    char    last_c = '\n';
    while (c != EOF)
    {
        if (c == '\n' && last_c != '\n')
        {
            ++lines;
        }

        last_c = c;
        c = fgetc(fs);
    }
    fseek(fs, 0, SEEK_SET);

    return lines;
}
*/