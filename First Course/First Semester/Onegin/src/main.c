#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

#include "../include/text.h"


void destructor(text_lines_st *lines, text_st text, FILE *fs1, FILE *fs2)
{
    freeTextLinesObject(lines);
    freeTextObject(text);
    fclose(fs1);
    fclose(fs2);
}

int main()
{
    // For cyrillic support
    SetConsoleOutputCP(1251);

    // Write the file data to the buffer and calculate the values we need 
    FILE *onegin_input_stream = fopen("../Onegin.txt", "rb");
    text_st text = getTextObject(onegin_input_stream);

    // Split text buffer into string lines
    text_lines_st *lines = getTextLinesObject(text);

    // Sort lines in lexicographic order
    lexicographicTextLinesSort(lines, text.lines_count);

    // Output sorted lines
    FILE *onegin_output_stream = fopen("../OneginOUTPUT.txt", "wb");
    saveTextLinesObject(lines, text.lines_count, onegin_output_stream);

    // Close all opened files and free allocated memory
    destructor(lines, text, onegin_input_stream, onegin_output_stream);

    return 0;
}