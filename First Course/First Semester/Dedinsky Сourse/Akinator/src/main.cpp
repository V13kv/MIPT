#include <stdio.h>
#include <stdbool.h>

#define DEBUG_LEVEL 1
#include "../lib/debug/debug.h"

#include "../include/akinator.h"
#include "../include/tree.h"

void hint();

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
        PRINT_ERROR_TRACING_MESSAGE(TREE_EXIT_CODES::ERROR_DURING_TREE_CONSTRUCTION_FROM_DATABASE);
        return EXIT_FAILURE;
    }

    // Main loop
    AKINATOR_GAME_MODES mode = AKINATOR_GAME_MODES::EXIT;
    while (true)
    {
        // Read playing mode
        // TODO: treeDtor implement + paste here
        akinatorGetPlayMode(&mode);

        // Process playing mode
        if (mode != AKINATOR_GAME_MODES::EXIT)
        {
            // TODO: treeDtor implement + paste here
            akinatorPlayGame(mode, &tree, argv[1]);
        }
        else
        {
            break;
        }
    }

    return 0;
}

void hint()
{
    printf("program.exe databaseName | databaseName - filepath to database\n");
}
