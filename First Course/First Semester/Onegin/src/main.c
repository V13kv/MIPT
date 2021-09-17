#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

#include "../include/text.h"
#include "../include/lines.h"

/**
 * @brief Destructor function (free's allocated memory&& closes file streams)
 * 
 * @param lines 
 * @param text 
 * @param fs1 
 * @param fs2 
 */
void destructor(text_line_st *lines, text_st text, FILE *fs1, FILE *fs2);

int main()
{
    // For cyrillic support
    SetConsoleOutputCP(1251);

    // Write the file data to the buffer and calculate the values we need 
    FILE *onegin_input_stream = fopen("../Onegin.txt", "r");
    text_st text = getTextObject(onegin_input_stream);

    // Split text buffer into string lines
    text_line_st *lines = getTextLinesObject(text);
    //printSeveralTextLines(lines, 2, text.lines_count);

    // Sort lines in lexicographic order
    bubbleSort(lines, text.lines_count, directLinesComparison);
    //my_qsort(lines, 0, text.lines_count, directLinesComparison);
    //qsort(lines, text.lines_count, sizeof(text_line_st), directLinesComparison);

    // Output sorted lines
    FILE *onegin_output_stream = fopen("../OneginOUTPUT.txt", "w");
    saveTextLinesObject(lines, text.lines_count, onegin_output_stream);

    // Close all opened files and free allocated memory
    destructor(lines, text, onegin_input_stream, onegin_output_stream);

    return 0;
}

void destructor(text_line_st *lines, text_st text, FILE *fs1, FILE *fs2)
{
    freeTextLinesObject(lines);
    freeTextObject(text);
    fclose(fs1);
    fclose(fs2);
}