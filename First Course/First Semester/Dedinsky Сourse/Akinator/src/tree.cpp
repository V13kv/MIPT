#include <stdio.h>
#include <string.h>  // for strcpy

#define DEBUG_LEVEL 1
#include "../lib/debug/debug.h"

#include "../include/tree.h"

EXIT_CODES treeCtor(tree_t *tree, char *databaseFileName)
{
    // Error check
    if (tree == NULL || databaseFileName == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::PASSED_OBJECT_IS_NULLPTR);
        return EXIT_CODES::PASSED_OBJECT_IS_NULLPTR;
    }

    // Tree construction
    FILE *database = fopen(databaseFileName, "r");



    return EXIT_CODES::NO_ERRORS;
}

static EXIT_CODES databaseReadNode(FILE *database, treeNode_t *node)
{
    // Error check
    if (database == NULL || node == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::PASSED_OBJECT_IS_NULLPTR);
        return EXIT_CODES::PASSED_OBJECT_IS_NULLPTR;
    }

    // Read node
    char treeNodeValue[MAX_TREE_NODE_VALUE_STR_LENGTH] = {};
    int databaseCharsRead = 0;
    fscanf(database, " { \"%" TO_STR(MAX_DATABASE_TREE_VALUE_STR_LENGTH) "[^\"]\"%n ", treeNodeValue, &databaseCharsRead);

    // Check `databaseCharsRead`
    if (databaseCharsRead == 0)
    {
        if (ferror(database))
        {
            PRINT_ERROR_TRACING_MESSAGE(TREE_DATABASE_EXIT_CODES::DATABASE_READ_ERROR);
        }
        else if (feof(database))
        {
            PRINT_ERROR_TRACING_MESSAGE(TREE_DATABASE_EXIT_CODES::DATABASE_STREAM_IS_EMPTY);
        }
        else
        {
            PRINT_ERROR_TRACING_MESSAGE(TREE_DATABASE_EXIT_CODES::BAD_DATABASE_FORMAT);
        }

        return EXIT_CODES::BAD_OBJECT_PASSED;
    }

    // Allocate memory for new node
    node = (treeNode_t *) calloc(1, sizeof(treeNode_t));
    CHECK_CALLOC_RESULT(node);

    // Init new node `value` field
    strcpy(node->value, treeNodeValue);

    // Init new node branches (`left` && `right` fields)
    node->left = NULL;
    node->right = NULL;

    // TODO: continue implementation


    return EXIT_CODES::NO_ERRORS;
}

EXIT_CODES databaseRead(FILE *database, tree_t *tree)
{
    // Error check
    if (database == NULL || tree == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::PASSED_OBJECT_IS_NULLPTR);
        return EXIT_CODES::PASSED_OBJECT_IS_NULLPTR;
    }

    // Read database
    IS_ERROR(databaseReadNode(database, tree->root))
    {
        PRINT_ERROR_TRACING_MESSAGE(TREE_DATABASE_EXIT_CODES::ERROR_DURING_READING_DATABASE_TREE_NODE);
        return EXIT_CODES::BAD_FUNC_RESULT;
    }

    return EXIT_CODES::NO_ERRORS;
}
