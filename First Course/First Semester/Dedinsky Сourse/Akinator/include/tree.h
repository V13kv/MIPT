#ifndef TREE_H
#define TREE_H

#include <stdio.h>

#include "../lib/debug/debug.h"
#include "settings.h"

enum class TREE_EXIT_CODES
{
    ERROR_DURING_TREE_CONSTRUCTION_FROM_DATABASE,
    ERROR_DURING_ACTION_FUNCTION_CALL,
    FAIL_TO_PUSH_NODES_LEFT_CHILD,
    FAIL_TO_PUSH_NODES_RIGHT_CHILD,
};

enum class TREE_DATABASE_EXIT_CODES
{
    ERROR_DURING_READING_DATABASE_TREE_NODE,
    ERROR_DURING_READING_DATABASE_LEFT_NODE,
    ERROR_DURING_READING_DATABASE_RIGHT_NODE,
    DATABASE_READ_ERROR,
    DATABASE_STREAM_IS_EMPTY,
    BAD_DATABASE_FORMAT,
    TREE_EXPORT_ERROR,
    ERROR_DURING_EXPORTING_TREE_TO_DATABASE,
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

typedef int (*actionFunction)(treeNode_t *node, void *arg);

EXIT_CODES treeNodeInit(treeNode_t **node, treeNodeElem_t *value = NULL);
EXIT_CODES treeCtorFromDatabase(tree_t *tree, char *databaseFileName);
EXIT_CODES treeExportToDatabase(tree_t *tree, char *databaseFileName);

EXIT_CODES treeDFSWActionFunction(tree_t *tree, actionFunction func, void *arg = NULL);
int treeBFSSearch(tree_t *tree, treeNodeElem_t *searchValue);


#endif  // TREE_H
