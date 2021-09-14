#include <stdio.h>

#include "../include/text.h"


int getTextSize(FILE *fs)
{
    fseek(fs, 0, SEEK_END);
    int fsize = ftell(fs);
    fseek(fs, 0, SEEK_SET);

    return fsize;
}

int getTotalAmountOfLines(char *text_data, int text_size)
{
    int total_line_numbers = 0;
    for (int index = 0; index < text_size; ++index)
    {
        //FIXME:? maybe there will be not a char, but address???...
        if (text_data[index] == '\n')
        {
            ++total_line_numbers;
        }
    }

    return total_line_numbers;
}