// TODO: support of multiple childs of nodes
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

#define DEBUG_LEVEL 1
#include "../lib/debug/debug.h"

#include "../include/akinator.h"
#include "../include/tree.h"

void hint();
void cleanUp(tree_t *tree);

// All functions that can be used here from akinator are returning error exit code (can be processed)
int main(int argc, char **argv)
{
    // Check argv count
    if (argc != 2)
    {
        hint();
    }

    // Create tree from database
    tree_t tree = {};
    IS_ERROR(treeCtorFromDatabase(&tree, argv[1]))
    {
        PRINT_ERROR_TRACING_MESSAGE(TREE_EXIT_CODES::TREE_CTOR_ERROR);
        return EXIT_FAILURE;
    }

    // Main loop
    char line[MAX_LINE_STR_LENGTH + 1] = {};
    AKINATOR_GAME_MODES mode = AKINATOR_GAME_MODES::EXIT;
    while (true)
    {
        // Read playing mode
        IS_ERROR(akinatorGetPlayMode(&mode))
        {
            cleanUp(&tree);
        }

        // Process playing mode
        if (mode != AKINATOR_GAME_MODES::EXIT)
        {
            IS_ERROR(akinatorPlayGame(mode, &tree, argv[1]))
            {
                cleanUp(&tree);
            }
        }
        else
        {
            break;
        }

        printf(TURN_TO_YELLOW("Press any char key to continue..."));
        READ_LINE_UNTIL_CORRECT_INPUT(readLine(line));
    }

    // Cleaning
    cleanUp(&tree);

    return 0;
}

void cleanUp(tree_t *tree)
{
    // Error check
    assert(tree != NULL && "[!] Passed object is nullptr!");

    // Clean up
    assert(treeDtor(tree) == EXIT_CODES::NO_ERRORS && "[!] Deconstruction error!");

    return;
}

void hint()
{
    printf("program.exe databaseName | databaseName - filepath to database\n");
}
