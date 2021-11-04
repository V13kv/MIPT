#include "../../lib/text/include/text.h"
#include "../../lib/colors/colors.h"

#include "../../include/processor/processor.h"

#define CLEAN_UP(textObj, cpuObj)   \
    textDtor(textObj);              \
    IS_OK_W_EXIT(cpuDtor(cpuObj));

#define EXIT(exitCode, message)             \
    PRINT_ERROR_TRACING_MESSAGE(message);   \
    exit(exitCode);

void hint();
char *getFileName(int argc, char **argv);

int main(int argc, char **argv)
{
    // Read bytecode
    text_t byteCode = {};
    textCtor(&byteCode, getFileName(argc, argv), FILE_MODE::RB);

    // Processor initialization
    cpu_t CPU = {};
    IS_ERROR(cpuCtor(&CPU))
    {
        CLEAN_UP(&byteCode, &CPU);
        EXIT(EXIT_FAILURE, EXIT_CODES::CONSTRUCTOR_ERROR);
    }

    // Execute bytecode
    IS_ERROR(cpuExecuteBytecode(&byteCode, &CPU))
    {
        CLEAN_UP(&byteCode, &CPU);
        EXIT(EXIT_FAILURE, PROCESSOR_EXIT_CODES::BYTES_EXECUTION_FAILURE);
    }

    // Free allocated space
    textDtor(&byteCode);
    IS_ERROR(cpuDtor(&CPU))
    {
        CLEAN_UP(&byteCode, &CPU);
        EXIT(EXIT_FAILURE, EXIT_CODES::DESTRUCTOR_ERROR);
    }

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
