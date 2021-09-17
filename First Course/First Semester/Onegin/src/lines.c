#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

#include "../include/lines.h"


text_line_st* getTextLinesObject(text_st text)
{
    // Allocate memory for each line of a text
    text_line_st *lines = malloc(sizeof(text_line_st) * text.lines_count);
    assert(lines != NULL && "[!] Got a null pointer after malloc function!");

    // Fill each line structure of the lines array
    char *text_p = text.data;
    assert(text_p != NULL && "[!] You have passed a null pointer as a text.data!");

    for (int line = 0; line < text.lines_count; ++line)
    {
        lines[line].beginning = text_p;
        lines[line].length = strlen(text_p);

        text_p += lines[line].length + 1;
    }

    return lines;
}

void saveTextLinesObject(const text_line_st *const lines, const int count_lines, FILE *fs)
{
    assert(fs != NULL && "[!] You have passed a null pointer as a file stream!");

    for (int line = 0; line < count_lines; ++line)
    {
        fwrite(lines[line].beginning, sizeof(char), lines[line].length, fs);
        fputc('\n', fs);
    }
}

void freeTextLinesObject(text_line_st *lines)
{
    free(lines);
}


void printSeveralTextLines(const text_line_st *const lines, const int lines_to_print, const int total_lines)
{
    assert(lines_to_print <= total_lines && "[!] You are trying to print lines more than you have!");

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
    assert(a != NULL && "[!] You have passed a null pointer as an 'a' parameter");
    assert(b != NULL && "[!] You have passed a null pointer as an 'b' parameter");

    const text_line_st *line1 = (text_line_st *) a;
    const text_line_st *line2 = (text_line_st *) b;

    char *l1p = line1->beginning;
    char *l2p = line2->beginning;
    assert(l1p != NULL && "[!] You have passed a null pointer as a line1->beginning!");
    assert(l2p != NULL && "[!] You have passed a null pointer as a line2->beginning");

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

void my_strrev(char *str, int len)
{
    assert(str != NULL && "[!] You have passed a null pointer as an 'str' parameter");

    for (int i = 0, j = len - 1; i < j; ++i, --j)
    {
        int a = str[i];
        str[i] = str[j];
        str[j] = a;
    }
}

int reversedLinesComparison(const void *a, const void *b)
{
    assert(a != NULL && "[!] You have passed a null pointer as an 'a' parameter");
    assert(b != NULL && "[!] You have passed a null pointer as an 'b' parameter");

    text_line_st *line1 = (text_line_st *) a;
    text_line_st *line2 = (text_line_st *) b;

    my_strrev(line1->beginning, line1->length);
    my_strrev(line2->beginning, line2->length);

    int res = directLinesComparison((void *) a, (void *)b);

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


int partition(text_line_st *lines, int low, int high, int (*comp)(const void*, const void *))
{
	text_line_st pivot = lines[high];
	int i = low - 1;

	for (int j = low; j <= high - 1; j++)
	{
		if ((*comp)((void *) &lines[j], (void *) &pivot) == FIRST_LINE_IS_LOWER)
		{
			i++;
			swap(&lines[i], &lines[j]);
		}
	}
	swap(&lines[i + 1], &lines[high]);
	return i + 1;
}

void my_qsort(text_line_st *lines, int low, int high, int (*comp)(const void *, const void *))
{
    assert(lines != NULL && "[!] You have passed a null pointer as a 'lines' parameter!");
    assert(comp != NULL && "[!] You have passed a null pointer as a 'comp' function!");

    if (low < high)
    {
        int partitioning_index = partition(lines, low, high, comp);

        my_qsort(lines, low, partitioning_index - 1, comp);
		my_qsort(lines, partitioning_index + 1, high, comp);
    }
}

void bubbleSort(text_line_st *lines, int lines_count, int (*comp)(void const *, void const*))
{
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