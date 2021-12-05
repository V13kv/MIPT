#ifndef TREE_H
#define TREE_H

#include "../lib/debug/debug.h"
#include "settings.h"

enum class TREE_EXIT_CODES
{
};

enum class TREE_DATABASE_EXIT_CODES
{
    ERROR_DURING_READING_DATABASE_TREE_NODE,
    DATABASE_READ_ERROR,
    DATABASE_STREAM_IS_EMPTY,
    BAD_DATABASE_FORMAT,
};

struct treeNode_t
{
    treeNode_t *left = NULL;
    treeNode_t *right = NULL;

    treeNodeElem_t value[MAX_TREE_NODE_VALUE_STR_LENGTH] = {};
};

struct tree_t
{
    treeNode_t *root = NULL;
};

#define TO_STR(object) STRINGIFY(object)

EXIT_CODES databaseRead(FILE *database, tree_t *tree);

EXIT_CODES treeCtor(tree_t *tree, char *databaseFileName);


#endif  // TREE_H
