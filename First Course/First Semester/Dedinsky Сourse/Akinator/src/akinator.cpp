#include <stdio.h>
#include <string.h>  // for memset && strcpy

#define DEBUG_LEVEL 1
#include "../lib/debug/debug.h"
#include "../lib/colors/colors.h"

#include "../include/akinator.h"
#include "../include/settings.h"

#define TURN_TO_YELLOW(text)    YELLOW text RESET

void akinatorModesHint()
{
    printf("Modes: "    TURN_TO_YELLOW("[G]") "uess | "
                        TURN_TO_YELLOW("[D]") "ump | "
                        TURN_TO_YELLOW("[E]") "xit\n");
}

static EXIT_CODES readLine(char *line)
{
    // Error check
    if (line == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::PASSED_OBJECT_IS_NULLPTR);
        return EXIT_CODES::PASSED_OBJECT_IS_NULLPTR;
    }

    // Reset give `line` object
    memset(line, DEFAULT_MEMSET_VALUE, MAX_LINE_STR_LENGTH);

    // Read value
    int charsRead = 0;
    int ret = scanf(DEFAULT_LINE_READ_REGEXP(MAX_LINE_STR_LENGTH), line, &charsRead);
    CHECK_SSCANF_RESULT(ret);

    return EXIT_CODES::NO_ERRORS;
}

EXIT_CODES akinatorGetPlayMode(AKINATOR_GAME_MODES *mode)
{
    // Error check
    if (mode == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::PASSED_OBJECT_IS_NULLPTR);
        return EXIT_CODES::PASSED_OBJECT_IS_NULLPTR;
    }

    // Modes hint
    system(CLEAR_CONSOLE_COMMAND);
    akinatorModesHint();

    // Get play mode
    char choiceMode = 0;
    char line[MAX_LINE_STR_LENGTH + 1] = {};

    EXIT_CODES READLINE_EXIT_CODE = EXIT_CODES::NO_ERRORS;
    while ((READLINE_EXIT_CODE = readLine(line)) == EXIT_CODES::NO_ERRORS)
    {
        // Input
        int ret = sscanf(line, "%c", &choiceMode);
        CHECK_SSCANF_RESULT(ret);

        // Get inputed mode
        switch (choiceMode)
        {
            case (char) AKINATOR_GAME_MODES::GUESS:
            case (char) AKINATOR_GAME_MODES::DUMP:
            case (char) AKINATOR_GAME_MODES::EXIT:
                *mode = (AKINATOR_GAME_MODES) choiceMode;
                goto MODE_SUCCESSFULLY_READ;
            default:
                break;
        }
        fflush(stdout);
        system(CLEAR_CONSOLE_COMMAND);

        // Hint
        akinatorModesHint();
    }
    return READLINE_EXIT_CODE;

    MODE_SUCCESSFULLY_READ:
        return EXIT_CODES::NO_ERRORS;
}

EXIT_CODES akinatorPlayGame(AKINATOR_GAME_MODES mode, tree_t *tree)
{
    // Error check
    if (tree == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::PASSED_OBJECT_IS_NULLPTR);
        return EXIT_CODES::PASSED_OBJECT_IS_NULLPTR;
    }

    // Start game according to the game `mode`
    switch (mode)
    {
        case AKINATOR_GAME_MODES::GUESS:
            // akinatorGuessPlay(tree);
            break;
        case AKINATOR_GAME_MODES::DUMP:
            akinatorDumpPlayTree(tree);
            break;
        default:
            PRINT_ERROR_TRACING_MESSAGE(AKINATOR_EXIT_CODES::AKINATOR_UNKNOWN_PLAY_MODE);
            return EXIT_CODES::BAD_OBJECT_PASSED;
    }

    return EXIT_CODES::NO_ERRORS;
}

EXIT_CODES akinatorDumpPlayTree(tree_t *tree)
{
    // Error check
    if (tree == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::PASSED_OBJECT_IS_NULLPTR);
        return EXIT_CODES::PASSED_OBJECT_IS_NULLPTR;
    }

    // Output file name
    char filename[MAX_LINE_STR_LENGTH + 1] = {};  // Count '\0'
    IS_ERROR(readLine(filename))
    {
        PRINT_ERROR_TRACING_MESSAGE(AKINATOR_EXIT_CODES::ERROR_READING_LINE);
        return EXIT_CODES::BAD_FUNC_RESULT;
    }

    // Add extension to inputed output file name
    char fileNameWExtension[MAX_LINE_STR_LENGTH + 4 + 1] = {}; // Count ".dot" (including '\0')
    strcpy(fileNameWExtension, filename);
    strcat(fileNameWExtension, OUTPUT_DUMP_FILE_EXTENSION);

    // Open || create output file
    FILE *outStream = fopen(filename, "w");
    CHECK_FOPEN_RESULT(outStream);

    // Dump
    fprintf(outStream, "digraph Tree {\n");

    // TODO: implement DFS algorithm to construct objects for GRAPHVIZ

    fprintf(outStream, "}\n");
    
    // Destruction
    fclose(outStream);

    // Configure command for graph program (GRAPHVIZ)
    char command[MAX_COMMAND_LENGTH + 1] = {};
    snprintf(command, MAX_COMMAND_LENGTH, COMMAND_FOR_GRAPH_DUMP_CONSTRUCTION, fileNameWExtension, filename);

    // Execute configured command
    system(command);

    return EXIT_CODES::NO_ERRORS;
}
