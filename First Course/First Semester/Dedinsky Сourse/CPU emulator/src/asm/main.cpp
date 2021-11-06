#include "../../lib/text/include/text.h"
#include "../../lib/colors/colors.h"

#include "../../include/asm/assembler.h"

void hint();
char *getFileName(int argc, char **argv, int fileIndex);

int main(int argc, char **argv)
{
    text_t code = {};
    textCtor(&code, getFileName(argc, argv, 1), FILE_MODE::R);

    assembly(&code, getFileName(argc, argv, 2));

    textDtor(&code);
    return 0;
}

void hint()
{
    printf(RED "Incorrect inline argument input!\n" RESET);
    printf("prog.exe <file_name> <output_file_name>\n");
}

char *getFileName(int argc, char *argv[], int fileIndex)
{
    char *file_name = NULL;
    if (argc >= fileIndex + 1)
    {
        file_name = argv[fileIndex];
    }
    else
    {
        hint();
    }

    return file_name;
}
