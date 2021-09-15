#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "../include/text.h"


void destructor(text_lines_t *lines, text_t text, FILE *fs)
{
    freeTextLinesObject(lines);
    freeTextObject(text);
    fclose(fs);
}

int main()
{
    SetConsoleOutputCP(65001);

    // Write the file data to the buffer and calculate the values we need 
    FILE *onegin_stream = fopen("../Onegin.txt", "r");
    text_t text = getTextObject(onegin_stream);
    printTextObject(text);

    // Split text buffer into string lines
    text_lines_t *lines = getTextLinesObject(text);
    printSeveralTextLines(lines, 3);

    // Sort strings in lexicographic order
    lexicographicTextLinesSort(lines, 3);//text.lines_count);

    // TODO: 1 выходной файл с отсортированными строками


    // Close all opened files and free allocated memory
    destructor(lines, text, onegin_stream);


    return 0;
}