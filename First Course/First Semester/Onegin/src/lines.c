#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include "../include/lines.h"

/**
 * @brief Extract lines from text using lines structure
 * 
 * @param text 
 * @return text_lines_st* 
 */
text_lines_st* getTextLinesObject(text_st text)
{
    // Allocate memory for each line of a text
    text_lines_st *lines = malloc(sizeof(text_lines_st) * text.lines_count);

    // Fill each line structure of the lines array
    char *text_p = text.data;
    for (int line = 0; line < text.lines_count; ++line)
    {
        lines[line].beginning = text_p;
        lines[line].length = strlen(text_p);

        text_p += lines[line].length + 1;
    }

    return lines;
}

/**
 * @brief Printing 'lines_to_print' lines from the beginning of the text
 * 
 * @param lines 
 * @param lines_to_print 
 * @param total_lines 
 */
void printSeveralTextLines(text_lines_st *lines, int lines_to_print, int total_lines)
{
    assert(lines_to_print <= total_lines && "[!] You are trying to print lines more than you have!");

    putchar('\n');
    for (int line = 0; line < lines_to_print; ++line)
    {
        printf("#%d line: %s\n", line + 1, lines[line].beginning);
        printf("length = %d\n\n", lines[line].length);
    }
}

/**
 * @brief Free the memory allocated for the lines of the file
 * 
 * @param lines 
 */
void freeTextLinesObject(text_lines_st *lines)
{
    free(lines);
}

/**
 * @brief Writing line objects to file
 * 
 * @param lines 
 * @param count_lines 
 * @param fs 
 */
void saveTextLinesObject(text_lines_st *lines, int count_lines, FILE *fs)
{
    for (int line = 0; line < count_lines; ++line)
    {
        fwrite(lines[line].beginning, sizeof(char), lines[line].length, fs);
        fputc('\n', fs);
        ++line;
    }
}



//TODO: optimize
int normalComparison(const void* a, const void* b)
{
    text_lines_st *line1 = (text_lines_st *) a;
    text_lines_st *line2 = (text_lines_st *) b;

    char *l1p = line1->beginning;
    char *l2p = line2->beginning;
    while (*l1p != '\0' && *l2p != '\0')
    {
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

//TODO: maybe optimize it: reverse lines, then call normalComparison
int reversedComparison(const void* a, const void* b)
{
    text_lines_st *line1 = (text_lines_st *) a;
    text_lines_st *line2 = (text_lines_st *) b;

    char *l1p = line1->beginning + line1->length - 1;
    char *l2p = line2->beginning + line2->length - 1;
    while (*l1p != *line1->beginning && *l2p != *line2->beginning)
    {
        if (*l1p < *l2p)
        {
            return -1;
        }
        else if (*l1p > *l2p)
        {
            return 1;
        }

        --l1p;
        --l2p;
    }

    if (*l1p == *line1->beginning && *l2p == *line2->beginning)
    {
        if (*l1p < *l2p)
        {
            return -1;
        }
        else if (*l2p < *l1p)
        {
            return 1;
        }
        else
        {
            return 0;
        }
    }
    else if (*l1p == *line1->beginning)
    {
        if (*l1p <= *l2p)
        {
            return -1;
        }
        else
        {
            return 1;
        }
    }
    else
    {
        if (*l2p <= *l1p)
        {
            return -1;
        }
        else
        {
            return 1;
        }
    }
}


/*
TODO: own quick_sort
void swap(text_lines_st *line1, text_lines_st *line2)
{
    text_lines_st temp = *line1;
    *line1 = *line2;
    *line2 = temp;
}

int partition(text_lines_st *lines, int low, int high, int (*comp)(const void*, const void *))
{
	text_lines_st *pivot = &lines[high];
	int i = (low - 1);

	for (int j = low; j <= high - 1; j++)
	{
        int res = (*comp)((void *) &lines[j], (void *) &pivot);
		if (res == -1)  //lines[j] < pivot)
		{
			i++;
			swap(&lines[i], &lines[j]);
		}
	}
	swap(&lines[i + 1], &lines[high]);
	return (i + 1);
}

void my_qsort(text_lines_st *lines, int low, int high, int (*comp)(const void *, const void *))
{
    if (low < high)
    {
        int partitioning_index = partition(lines, low, high, comp);

        my_qsort(lines, low, partitioning_index - 1, comp);
		my_qsort(lines, partitioning_index + 1, high, comp);
    }
}
*/