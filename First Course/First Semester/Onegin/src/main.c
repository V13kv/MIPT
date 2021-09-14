#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "../include/text.h"


typedef struct 
{
    char *beginning;
    int length;
} text_line_t;

typedef struct 
{
    char *data;
    const int size;
    const int total_line_numbers;
} text_t;

//FIXME:? может до этой функции (в функциях getTextSize и getTotalAmountOfLines и т.д) считать кол-во строк != только \n?
// или сделать beautify до этого?
void getLinesFromText(text_t text, text_line_t *lines)//text_t text)//const char *data, text_t text)
{
    for (int line_number = 0; line_number < text.total_line_numbers; ++line_number)
    {
        lines[line_number].beginning = text.data;

        int line_length = 0;
        while (*text.data != '\n')
        {
            line_length++;

            text.data++;
        }
        text.data++;

        //FIXME:? мы выделяли память даже под строки, состоящие из только \n, получается, что тогда мы не используем
        // некоторую часть выделенной памяти памяти
        //if (line_length > 0)
        //{
        lines[line_number].length = line_length + 1;  // Сохраняем длину строки + \n
        //}
    }
}

void printLines(text_line_t *lines, int lines_to_print)
{
    for (int i = 0; i < lines_to_print; ++i)
    {
        for (char *l = lines[i].beginning; l < lines[i].beginning + lines[i].length; ++l)
        {
            printf("%c", *l);
        }
        putchar('\n');
    }
}

int main()
{
    // Записать данные файла в буфер и вычислить нужные нам величины 
    FILE *onegin_stream = fopen("../Onegin.txt", "r");

    const int text_size = getTextSize(onegin_stream);

    char *text_data = (char *) malloc(sizeof(char) * (text_size + 1));
    text_data[text_size] = '\0';
    fread(text_data, sizeof(char), text_size, onegin_stream);

    //TODO:? maybe beautify text_data here (delete empty lines etc. => need to change somehow getTextSize())

    const int total_line_numbers = getTotalAmountOfLines(text_data, text_size);
    
    text_t text = {text_data, text_size, total_line_numbers};
    //printf("text_size: %d\ntotal_line_numbers: %d\n", text.size, text.total_line_numbers);



    // Разделить буфер на строки
    text_line_t *lines = malloc(sizeof(text_line_t) * text.total_line_numbers);
    getLinesFromText(text, lines);
    printLines(lines, 1);



    // Отсортировать строки в лексикографическом порядке

    // 1 выходной файл с отсортированными строками
    
    // Закрыть открытые файлы и освободить выделеннуб динамически память
    free(text_data);
    free(lines);
    fclose(onegin_stream);

    return 0;
}