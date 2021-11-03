#include "../../lib/text/include/text.h"
#include "../../lib/colors/colors.h"

#include "../../include/processor/processor.h"

void hint();
char *getFileName(int argc, char **argv);

int main(int argc, char **argv)
{
    // Read bytecode
    text_t byteCode = {};
    textCtor(&byteCode, getFileName(argc, argv), FILE_MODE::RB);

    // Processor initialization
    cpu_t CPU = {};
    IS_OK_W_EXIT(cpuCtor(&CPU));

    // Execute bytecode
    IS_OK_W_EXIT(cpuExecuteBytecode(&byteCode, &CPU));

    // Free allocated space
    IS_OK_W_EXIT(cpuDtor(&CPU));
    textDtor(&byteCode);

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
