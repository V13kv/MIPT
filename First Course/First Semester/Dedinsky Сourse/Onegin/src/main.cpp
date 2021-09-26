#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
    #include <Windows.h>  // only for SetConsoleOutputCP function
#endif

#include "../include/text.h"
#include "../include/lines.h"

//FIXME: combine test_st and text_line_st
//TODO: replace malloc on calloc on Linux and etc. (do macros, e.g. )
int main()
{
    #ifdef _WIN32
        // For cyrillic support
        SetConsoleCP(1251);
        SetConsoleOutputCP(1251);
    #endif

    // Create text object
    text_st text = {};
    text_st_constructor(&text, "../Onegin.txt");  // TODO: Add parameters to interact with lines

    // Get text lines
    text_line_st *lines = get_text_lines(&text);

    // Sort lines in lexicographic order
    my_qsort(lines, 0, text.lines_count, reversedLinesComparison);
    //qsort(lines, text.lines_count, sizeof(text_line_st), directLinesComparison);

    // Save sorted lines to files
    export_text_line_objects(lines, text.lines_count, "../OneginOUTPUT.txt");  // TODO: выходной файл с трёмя текстами (direct, reversed, original)

    // Close all opened files and free allocated memory
    text_st_deconstructor(&text);
    free(lines);

    return 0;
}
