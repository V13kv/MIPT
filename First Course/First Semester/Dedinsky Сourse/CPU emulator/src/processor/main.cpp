#include "../../lib/text/include/text.h"
#include "../../lib/stack/include/stack.h"

void hint();
char *getFileName(int argc, char *argv[]);

int main(int argc, char *argv[])
{
    // Read translated byteCode
    text_t byteCode = {};
    textCtor(&byteCode, getFileName(argc, argv), FILE_MODE::RB);

    // Create stack
    stack_t stack = {};
    stackCtor(&stack);

    // Create RAM
    ram_t ram = {};
    ramCtor(&ram);


    textDtor(&byteCode);
    stackDtor(&stack);

    return 0;
}


void hint()
{
    printf(RED "Incorrect inline argument input!\n" RESET);
    printf("disasm.exe <file_name_to_disas>\n");
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

