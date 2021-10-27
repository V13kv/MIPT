#include "../../lib/text/include/text.h"
#include "../../include/processor/processor.h"

void hint();
char *getFileName(int argc, char *argv[]);

int main(int argc, char *argv[])
{
    // Read bytecode
    text_t byteCode = {};
    textCtor(&byteCode, getFileName(argc, argv), FILE_MODE::RB);

    // Init processor
    processor_t *cpu = {};
    cpuCtor(&cpu);

    // Execution
    // TODO: implementation
    //execute(&byteCode, &cpu);

    textDtor(&byteCode);
    cpuDtor(&cpu);

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

