#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>  // only for SetConsoleOutputCP function

#include "../include/text.h"
#include "../include/lines.h"


int main()
{
    // For cyrillic support
    SetConsoleOutputCP(1251);

    // Create text object
    text_st text = {0, 0, 0};
    text_st_constructor(&text, "../Onegin.txt");

    // Get text lines
    text_line_st *lines = get_text_lines(&text);

    // Sort lines in lexicographic order
    my_qsort(lines, 0, text.lines_count, directLinesComparison);
    //qsort(lines, text.lines_count, sizeof(text_line_st), directLinesComparison);

    // Save sorted lines to files
    export_text_line_objects(lines, text.lines_count, "../OneginOUTPUT.txt");

    // Close all opened files and free allocated memory
    text_st_deconstructor(&text);
    free(lines);

    return 0;
}