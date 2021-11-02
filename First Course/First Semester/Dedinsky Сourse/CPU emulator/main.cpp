#include "lib/text/include/text.h"
#include "lib/colors/colors.h"

#include "include/asm/assembler.h"

void hint();
char *getFileName(int argc, char **argv);

int main(int argc, char **argv)
{
    text_t code = {};
    textCtor(&code, getFileName(argc, argv), FILE_MODE::R);

    assembly(&code, "asm.bin");

    textDtor(&code);
    return 0;
}

void hint()
{
    printf(RED "Incorrect inline argument input!\n" RESET);
    printf("prog.exe <file_name>\n");
}

char *getFileName(int argc, char *argv[])
{
    char *file_name = NULL;
    if (argc == 2)
    {
        file_name = argv[1];
    }
    else
    {
        hint();
    }

    return file_name;
}
