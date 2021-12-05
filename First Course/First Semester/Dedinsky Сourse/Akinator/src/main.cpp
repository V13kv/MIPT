#include <stdio.h>
#include <stdbool.h>

#include "../include/akinator.h"
#include "../include/tree.h"

void hint();

int main(int argc, char **argv)
{
    // Check argv count
    if (argc != 2)
    {
        hint();
    }

    // Construct tree using database
    tree_t tree = {};
    IS_ERROR(treeCtor(&tree, argv[1]))
    {
        return EXIT_FAILURE;
    }

    // Main loop
    AKINATOR_GAME_MODES mode = AKINATOR_GAME_MODES::EXIT;
    while (true)
    {
        // Read playing mode
        akinatorGetPlayMode(&mode);

        // Process playing mode
        if (mode == AKINATOR_GAME_MODES::EXIT)
        {
            break;
        }
        else
        {
            akinatorPlayGame(mode, &tree);
        }
    }

    return 0;
}

void hint()
{
    printf("program: databaseName\n\tdatabaseName - filepath to database\n");
}
