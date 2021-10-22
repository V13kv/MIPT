#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
    #include <Windows.h>  // only for SetConsoleOutputCP function
#endif

#include "../include/text.h"

#include "../include/lines_sort.h"

int main()
{
    // For cyrillic support
    #ifdef _WIN32
        SetConsoleCP(1251);
        SetConsoleOutputCP(1251);
    #endif

    // Create text object
    text_t text = {};
    textCtor(&text, "Onegin.txt", FILE_MODE::R);

    // Save original text
    exportTextObject(&text, "OneginOUTPUT.txt", FILE_MODE::W);

    // Sort lines in direct lexicographic order && Save sorted lines to files
    my_qsort(&text, directLinesComparison);
    exportTextObject(&text, "OneginOUTPUT.txt", FILE_MODE::A);

    // Sort lines in direct lexicographic order && Save sorted lines to files
    my_qsort(&text, reversedLinesComparison);
    exportTextObject(&text, "OneginOUTPUT.txt", FILE_MODE::A);

    textDtor(&text);

    return 0;
}
