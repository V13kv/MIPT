#include <assert.h>
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
            // IS_ERROR(akinatorGuessPlay(tree))
            // {
            //     PRINT_ERROR_TRACING_MESSAGE(AKINATOR_EXIT_CODES::ERROR_DURING_GUESS_PLAY);
            //     return EXIT_CODES::BAD_FUNC_RESULT;
            // }
            break;
        case AKINATOR_GAME_MODES::DUMP:
            // TODO: CHECK FOR CORRECTNESS
            IS_ERROR(akinatorDumpPlayTree(tree))
            {
                PRINT_ERROR_TRACING_MESSAGE(AKINATOR_EXIT_CODES::ERROR_DURING_TREE_DUMP);
                return EXIT_CODES::BAD_FUNC_RESULT;
            }
            break;
        default:
            PRINT_ERROR_TRACING_MESSAGE(AKINATOR_EXIT_CODES::AKINATOR_UNKNOWN_PLAY_MODE);
            return EXIT_CODES::BAD_OBJECT_PASSED;
    }

    return EXIT_CODES::NO_ERRORS;
}

// FIXME: maybe two if statements are not work properly (could be 2 same lines with left and right child for one node, test what happens)
static int dumpNode(treeNode_t *node, FILE *outStream)
{
    // Error check
    assert(node != NULL && outStream != NULL && "EXIT_CODES::PASSED_OBJECT_IS_NULLPTR");

    // Dump node && its branches
    int result = NODE_HAS_NOT_ANY_BRANCHES;
    if (node->left != NULL)
    {
        // Dump node with branches (left and right)
        fprintf(outStream, NODE_DUMP_W_LEFT_BRANCH_REGEXP, (void *) node, (void *) node->left, node->value);

        // Dump node branches
        fprintf(outStream, NODE_LEFT_BRANCH_DUMP_REGEXP, (void *) node, (void *) node->left);
    
        result |= NODE_HAS_LEFT_BRANCH;
    }
    
    if (node->right != NULL)
    {
        fprintf(outStream, NODE_DUMP_W_RIGHT_BRANCH_REGEXP, (void *) node, node->value, (void *) node->right);
        fprintf(outStream, NODE_RIGHT_BRANCH_DUMP_REGEXP, (void *) node, (void *) node->right);

        result |= NODE_HAS_RIGHT_BRANCH;
    }
    
    if (node->left == NULL && node->right == NULL)
    {
        // Dump node WO branches (left and right)
        fprintf(outStream, NODE_DUMP_WO_BRANCHES_REGEXP, (void *) node, node->value);

        result |= NODE_HAS_NOT_ANY_BRANCHES;
    }

    return result;
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
    IS_ERROR(treeDFSWActionFunction(tree, dumpNode, outStream))
    {
        PRINT_ERROR_TRACING_MESSAGE(AKINATOR_EXIT_CODES::ERROR_DUMPING_TREE_VIA_DFS_FUNCTION);
        return EXIT_CODES::BAD_FUNC_RESULT;
    }
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

// TODO: implementation
EXIT_CODES akinatorGuessPlay(tree_t *tree)
{
    // Error check
    if (tree == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::PASSED_OBJECT_IS_NULLPTR);
        return EXIT_CODES::PASSED_OBJECT_IS_NULLPTR;
    }

    // Guess play
    // IS_ERROR(treeDFSWActionFunction(tree, guessNode, ))


    return EXIT_CODES::NO_ERRORS;
}
