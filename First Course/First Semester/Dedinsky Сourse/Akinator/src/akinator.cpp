#include <stdio.h>
#include <string.h>  // for memset

#define DEBUG_LEVEL 1
#include "../lib/debug/debug.h"
#include "../lib/colors/colors.h"

#include "../include/akinator.h"

#define TURN_TO_YELLOW(text)    text//YELLOW text RESET

void akinatorModesHint()
{
    printf("Modes: "    TURN_TO_YELLOW("[G]") "uess | "
                        TURN_TO_YELLOW("[D]") "ump | "
                        TURN_TO_YELLOW("[E]") "xit\n");
}

static void clearStdinBuffer()
{
    int c = 0;
    while ((c = getchar()) != '\n' && c != EOF) { }
}

EXIT_CODES akinatorGetPlayMode(AKINATOR_GAME_MODES *mode)
{
    // Error check
    if (mode == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::PASSED_OBJECT_IS_NULLPTR);
        return EXIT_CODES::PASSED_OBJECT_IS_NULLPTR;
    }

    // Get play mode
    char input = DEFAULT_CHAR_VALUE;
    while (true)
    {
        akinatorModesHint();
        printf("INPUT> ");
        scanf(" %c", &input);

        switch (input)
        {
            case (char) AKINATOR_GAME_MODES::GUESS:
            case (char) AKINATOR_GAME_MODES::DUMP:
            case (char) AKINATOR_GAME_MODES::EXIT:
                *mode = (AKINATOR_GAME_MODES) input;
                goto exit;
            default:
                break;
        }
        clearStdinBuffer();
    }

    exit:
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
            akinatorGuessPlay(tree);
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
