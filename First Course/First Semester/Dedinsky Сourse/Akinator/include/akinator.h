#ifndef AKINATOR_H
#define AKINATOR_H

#include "../lib/debug/debug.h"
#include "settings.h"
#include "tree.h"

enum class AKINATOR_EXIT_CODES
{
    AKINATOR_UNKNOWN_PLAY_MODE,
    ERROR_READING_LINE,
    ERROR_DUMPING_TREE_VIA_DFS_FUNCTION,
    ERROR_DURING_TREE_DUMP,
    ERROR_DURING_GUESS_PLAY,
    BAD_INPUT_OF_SECRET_WORD,
    ERROR_DURING_GUESS_PLAY_DFS_FUNCTION,
    ERROR_DURING_GAME_PLAY,
    ERROR_TAKING_GAME_PLAY_MODE,
    ERROR_DURING_SEARCH_PLAY_MODE,
};

enum class AKINATOR_GAME_MODES
{
    EXIT    = 'E',
    GUESS   = 'G',
    DUMP    = 'D',
    SEARCH  = 'S',
};

void akinatorModesHint();

EXIT_CODES akinatorGetPlayMode(AKINATOR_GAME_MODES *mode);
EXIT_CODES akinatorPlayGame(AKINATOR_GAME_MODES mode, tree_t *tree, char *databaseFileName);

EXIT_CODES akinatorDumpPlayTree(tree_t *tree);
EXIT_CODES akinatorGuessPlay(tree_t *tree);
EXIT_CODES akinatorSearch(tree_t *tree);



#endif  // AKINATOR_H
