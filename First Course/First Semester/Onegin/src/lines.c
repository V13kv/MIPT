#include <assert.h>
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
    text_line_st *lines = malloc(sizeof(text_line_st) * text->lines_count);
    assert(lines != NULL && "[!] Got a null pointer after malloc function!");

    // Constructing lines
    char *text_p = text->data;
    assert(text_p != NULL && "[!] You have passed a null pointer as a text.data!");

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
        printf("length = %d\n\n", lines[line].length);
    }
}


bool isLetter(char chr)
{
    return  (chr >= 'A' && chr <= 'Z') || (chr >= 'a' && chr <= 'z') ||  // For english symbols
            (chr >= 'À' && chr <= 'ß') || (chr >= 'à' && chr <= 'ÿ');    // For russian symbols
}

int directLinesComparison(const void *a, const void *b)
{
    // Error check
    assert(a != NULL && "[!] You have passed a null pointer as an 'a' parameter");
    assert(b != NULL && "[!] You have passed a null pointer as an 'b' parameter");

    const text_line_st *line1 = (text_line_st *) a;
    const text_line_st *line2 = (text_line_st *) b;

    char *l1p = line1->beginning;
    char *l2p = line2->beginning;
    assert(l1p != NULL && "[!] You have passed a null pointer as a line1->beginning!");
    assert(l2p != NULL && "[!] You have passed a null pointer as a line2->beginning");

    // Compare line objects
    while (*l1p != '\0' && *l2p != '\0')
    {
        #ifdef CMP_LINES_WO_PUNCTUATION
            if (!isLetter(*l1p) || !isLetter(*l2p))
            {
                while (*l1p != '\0' && !isLetter(*l1p))
                {
                    ++l1p;
                }

                while (*l2p != '\0' && !isLetter(*l2p))
                {
                    ++l2p;
                }
            }
        #endif  // CMP_LINES_WO_PUNCTUATION

        if (*l1p < *l2p)
        {
            return -1;
        }
        else if (*l2p < *l1p)
        {
            return 1;
        }

        ++l1p;
        ++l2p;
    }

    if (*l1p == '\0' && *l2p == '\0')
    {
        return 0;
    }
    else if (*l1p == '\0')
    {
        return -1;
    }
    else
    {
        return 1;
    }
}

void my_strrev(char *const str, const int len)
{
    // Error check
    assert(str != NULL && "[!] You have passed a null pointer as an str parameter");
    assert(len > 0 && "[!] You have passed negative number as an len parameter");

    // String reversing
    for (int i = 0, j = len - 1; i < j; ++i, --j)
    {
        int a = str[i];
        str[i] = str[j];
        str[j] = a;
    }
}

int reversedLinesComparison(const void *a, const void *b)
{
    // Error check
    assert(a != NULL && "[!] You have passed a null pointer as an 'a' parameter");
    assert(b != NULL && "[!] You have passed a null pointer as an 'b' parameter");

    text_line_st *line1 = (text_line_st *) a;
    text_line_st *line2 = (text_line_st *) b;
    assert(line1->beginning != NULL && "[!] You have passed a null pointer as a line1->beginning!");
    assert(line2->beginning != NULL && "[!] You have passed a null pointer as a line2->beginning");

    // Lines reversing
    my_strrev(line1->beginning, line1->length);
    my_strrev(line2->beginning, line2->length);

    // Comparison
    int res = directLinesComparison((const void *) line1, (const void *) line2);

    // Reverse lines back
    my_strrev(line1->beginning, line1->length);
    my_strrev(line2->beginning, line2->length);

    return res;
}

void swap(text_line_st *line1, text_line_st *line2)
{
    assert(line1 != NULL && "[!] You have passed a null pointer as an 'line1' parameter");
    assert(line2 != NULL && "[!] You have passed a null pointer as an 'line2' parameter");

    text_line_st temp = *line1;
    *line1 = *line2;
    *line2 = temp;
}


int partition(text_line_st *const lines, int low, int high, int (*comp)(const void*, const void *))
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
        cmp_results_et res = (*comp)((const void *) &lines[j], (const void *) &pivot);
		if (res == CMP_FIRST_IS_LOWER)
		{
			i++;
			swap(&lines[i], &lines[j]);
		}
	}
	swap(&lines[i + 1], &lines[high]);

	return i + 1;
}

void _my_qsort(text_line_st *lines, int low, int high, int (*comp)(const void *, const void *))
{
    if (low < high)
    {
        int part_index = partition(lines, low, high, comp);

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