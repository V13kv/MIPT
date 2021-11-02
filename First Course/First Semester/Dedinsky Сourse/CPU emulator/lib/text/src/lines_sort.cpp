#include <assert.h>

#define CMP_LINES_WO_PUNCTUATION  // Used to exclude punctuation from lines comparison functions (comment/uncomment)
#include "../include/lines_sort.h"

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
    const text_line_t *lineA = (const text_line_t *) a;
    const text_line_t *lineB = (const text_line_t *) b;

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

    const text_line_t *lineA = (const text_line_t *) a;
    const text_line_t *lineB = (const text_line_t *) b;

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

void swap(text_line_t *line1, text_line_t *line2)
{
    assert(line1 != NULL && "[!] You have passed a null pointer as an 'line1' parameter");
    assert(line2 != NULL && "[!] You have passed a null pointer as an 'line2' parameter");

    text_line_t temp = *line1;
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
static int _partition(text_line_t *const lines, int low, int high, int (*comp)(const void*, const void *))
{
    // Error check
    assert(lines != NULL && "[!] You have passed a null pointer as a lines parameter!");
    assert(comp != NULL && "[!] You have passed a null pointer as a comp function!");
    assert(low >= 0 && "[!] You have passed negative low parameter!");
    assert(low <= high && "[!] low parameter is greater than high parameter!");

    // Partitioning
	text_line_t pivot = lines[high];
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
static void _my_qsort(text_line_t *lines, int low, int high, int (*comp)(const void *, const void *))
{
    if (low < high)
    {
        int part_index = _partition(lines, low, high, comp);

        _my_qsort(lines, low, part_index - 1, comp);
		_my_qsort(lines, part_index + 1, high, comp);
    }
}

void my_qsort(text_t *text, int (*comp)(const void *, const void *))
{
    // Error check
    assert(text->lines != NULL && "[!] You have passed a null pointer as a lines parameter!");
    assert(comp != NULL && "[!] You have passed a null pointer as a comp function!");

    // Quick sort
    _my_qsort(text->lines, 0, text->lines_count - 1, comp);
}

void bubbleSort(text_t *text, int (*comp)(void const *, void const*))
{
    // Error checking
    assert(text->lines != NULL && "[!] You have passed a null pointer as a lines array!");
    for (int line = 0; line < text->lines_count; ++line)
    {
        assert(text->lines[line].beginning != NULL && "[!] text_t structure has lines[ind].beginning as a null pointer");
    }
    assert(comp != NULL && "[!] You have passed a null pointer as a comp function!");

    // Bubble sort
    for (int i = 0; i < text->lines_count; ++i)
    {
        for (int j = i + 1; j < text->lines_count; ++j)
        {
            if ((*comp)((void *) &text->lines[j], (void *) &text->lines[i]) == -1)
            {
                swap(&text->lines[j], &text->lines[i]);
            }
        }
    }
}
