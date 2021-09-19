#include <assert.h>
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "../include/lines.h"


text_line_st* get_text_lines(text_st *text)
{
    // Error check
    assert(text != NULL && "[!] You have passed a null pointer as a text_st structure!");

    // Allocate memory for all lines
    text_line_st *lines = (text_line_st*) malloc(sizeof(text_line_st) * text->lines_count);
    assert(lines != NULL && "[!] Got a null pointer after malloc function!");

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

void export_text_line_objects(const text_line_st *const lines, const int lines_count, const char *file_name)
{
    // Create file_name file
    FILE *fs = fopen(file_name, "w");

    // Error check
    assert(fs != NULL && "[!] You have passed a null pointer as a file stream!");
    assert(lines != NULL && "[!] You have passed a null pointer as a lines array!");
    for (int line = 0; line < lines_count; ++line)
    {
        assert(lines[line].beginning != NULL && "[!] text_line_st structure has lines[ind].beginning as a null pointer");
    }
    assert(lines_count > 0 && "[!] You have passed negative lines_count parameter!");

    // Export
    for (int line = 0; line < lines_count; ++line)
    {
        fwrite(lines[line].beginning, sizeof(char), lines[line].length, fs);
        fputc('\n', fs);
    }

    // Cloes file stream object
    fclose(fs);
}

void printSeveralTextLines(const text_line_st *const lines, const int lines_to_print, const int total_lines)
{
    // Error check
    assert(lines != NULL && "[!] You have passed a null pointer as a lines array!");
    for (int line = 0; line < lines_to_print; ++line)
    {
        assert(lines[line].beginning != NULL && "[!] text_line_st structure has lines[ind].beginning as a null pointer");
    }
    assert(lines_to_print <= total_lines && "[!] You are trying to print lines more than you have!");

    // Print lines
    putchar('\n');
    for (int line = 0; line < lines_to_print; ++line)
    {
        printf("#%d line: %s\n", line + 1, lines[line].beginning);
        printf("length = %zu\n\n", lines[line].length);
    }
}

//TODO: use isalpha instead of mine (isalpha uses set_locale), inline
inline bool isLetter(const char chr)
{
    //return isalpha(chr);
    return  (chr >= 'A' && chr <= 'Z') || (chr >= 'a' && chr <= 'z') ||  // For english symbols
            (chr >= 'À' && chr <= 'ß') || (chr >= 'à' && chr <= 'ÿ');    // For russian symbols
}

void skip_letters(char **str, cmp_options_et cmp_option)
{
    while (**str != '\0' && !isLetter(**str))
    {
        *str = *str + cmp_option;
    }
}

int directLinesComparison(const void *a, const void *b)
{
    // Error check
    assert(a != NULL && "[!] You have passed a null pointer as an 'a' parameter");
    assert(b != NULL && "[!] You have passed a null pointer as an 'b' parameter");

    //lineA, lineB, strA, strB, lenA, lenB
    const text_line_st *lineA = (const text_line_st *) a;
    const text_line_st *lineB = (const text_line_st *) b;

    char *strA = lineA->beginning;
    char *strB = lineB->beginning;
    assert(strA != NULL && "[!] You have passed a null pointer as a line_a->beginning!");
    assert(strB != NULL && "[!] You have passed a null pointer as a line_b->beginning");

    //TODO: command line arguments
    while (*strA != '\0' && *strB != '\0')
    {
        #ifdef CMP_LINES_WO_PUNCTUATION
            if (!isLetter(*strA) || !isLetter(*strB))
            {
                skip_letters(&strA, CMP_OPTIONS_DIRECT);
                skip_letters(&strB, CMP_OPTIONS_DIRECT);
            }
        #endif  // CMP_LINES_WO_PUNCTUATION

        if (*strA < *strB)
        {
            return CMP_FIRST_IS_LOWER;
        }
        else if (*strB < *strA)
        {
            return CMP_FIRST_IS_GREATER;
        }

        ++strA;
        ++strB;
    }

    if (*strA == '\0' && *strB == '\0')
    {
        return CMP_ARE_EQUAL;
    }
    else if (*strA == '\0')
    {
        return CMP_FIRST_IS_LOWER;
    }
    else
    {
        return CMP_FIRST_IS_GREATER;
    }
}

int reversedLinesComparison(const void *a, const void *b)
{
    // Error check
    assert(a != NULL && "[!] You have passed a null pointer as an 'a' parameter");
    assert(b != NULL && "[!] You have passed a null pointer as an 'b' parameter");

    const text_line_st *lineA = (const text_line_st *) a;
    const text_line_st *lineB = (const text_line_st *) b;

    char *strA = lineA->beginning + lineA->length - 1;
    char *strB = lineB->beginning + lineB->length - 1;
    assert(strA != NULL && "[!] You have passed a null pointer as a lineA->beginning!");
    assert(strB != NULL && "[!] You have passed a null pointer as a lineB->beginning");

    // Compare line objects
    while (strA != lineA->beginning && strB != lineB->beginning)
    {
        #ifdef CMP_LINES_WO_PUNCTUATION
            if (!isLetter(*strA) || !isLetter(*strB))
            {
                skip_letters(&strA, CMP_OPTIONS_REVERSED);
                skip_letters(&strB, CMP_OPTIONS_REVERSED);
            }
        #endif  // CMP_LINES_WO_PUNCTUATION

        if (*strA < *strB)
        {
            return CMP_FIRST_IS_LOWER;
        }
        else if (*strB < *strA)
        {
            return CMP_FIRST_IS_GREATER;
        }

        --strA;
        --strB;
    }

    if (strA == lineA->beginning && strB == lineB->beginning)
    {
        return CMP_ARE_EQUAL;
    }
    else if (strA == lineA->beginning)
    {
        return CMP_FIRST_IS_LOWER;
    }
    else
    {
        return CMP_FIRST_IS_GREATER;
    }
}

void swap(text_line_st *line1, text_line_st *line2)
{
    assert(line1 != NULL && "[!] You have passed a null pointer as an 'line1' parameter");
    assert(line2 != NULL && "[!] You have passed a null pointer as an 'line2' parameter");

    text_line_st temp = *line1;
    *line1 = *line2;
    *line2 = temp;
}

/**
 * @brief Part of a my_qsort (quick sort) algorithm
 * 
 * @param lines 
 * @param low 
 * @param high 
 * @param comp 
 * @return int 
 */
static int _partition(text_line_st *const lines, int low, int high, int (*comp)(const void*, const void *))
{
    // Error check
    assert(lines != NULL && "[!] You have passed a null pointer as a lines parameter!");
    assert(comp != NULL && "[!] You have passed a null pointer as a comp function!");
    assert(low >= 0 && "[!] You have passed negative low parameter!");
    assert(low <= high && "[!] low parameter is greater than high parameter!");

    // Partitioning
	text_line_st pivot = lines[high];
	int i = low - 1;

	for (int j = low; j <= high - 1; j++)
	{
        cmp_results_et res = (cmp_results_et) (*comp)((const void *) &lines[j], (const void *) &pivot);
		if (res == CMP_FIRST_IS_LOWER)
		{
			i++;
			swap(&lines[i], &lines[j]);
		}
	}
	swap(&lines[i + 1], &lines[high]);

	return i + 1;
}

/**
 * @brief Quick sort algorithm implemented to sort line structures
 * 
 * @param lines 
 * @param low 
 * @param high 
 * @param comp 
 */
static void _my_qsort(text_line_st *lines, int low, int high, int (*comp)(const void *, const void *))
{
    if (low < high)
    {
        int part_index = _partition(lines, low, high, comp);

        _my_qsort(lines, low, part_index - 1, comp);
		_my_qsort(lines, part_index + 1, high, comp);
    }
}

void my_qsort(text_line_st *lines, int low, int high, int (*comp)(const void *, const void *))
{
    // Error check
    assert(lines != NULL && "[!] You have passed a null pointer as a lines parameter!");
    assert(comp != NULL && "[!] You have passed a null pointer as a comp function!");
    assert(low >= 0 && "[!] You have passed negative low parameter!");
    assert(low <= high && "[!] low parameter is greater than high parameter!");

    // Quick sort
    _my_qsort(lines, low, --high, comp);
}

void bubbleSort(text_line_st *lines, int lines_count, int (*comp)(void const *, void const*))
{
    // Error checking
    assert(lines != NULL && "[!] You have passed a null pointer as a lines array!");
    for (int line = 0; line < lines_count; ++line)
    {
        assert(lines[line].beginning != NULL && "[!] text_line_st structure has lines[ind].beginning as a null pointer");
    }
    assert(lines_count <= lines_count && "[!] You are trying to print lines more than you have!");
    assert(comp != NULL && "[!] You have passed a null pointer as a comp function!");

    // Bubble sort
    for (int i = 0; i < lines_count; ++i)
    {
        for (int j = i + 1; j < lines_count; ++j)
        {
            if ((*comp)((void *)&lines[j], (void *)&lines[i]) == -1)
            {
                swap(&lines[j], &lines[i]);
            }
        }
    }
}