#include <stdio.h>
#include <string.h>  // for strcpy

#include <assert.h>  // for assert

#define DEBUG_LEVEL 1
#include "../lib/debug/debug.h"

#include "../include/settings.h"
#include "../include/tree.h"

#include "../lib/stack/include/stack.h"
#include "../lib/list/include/list.h"

static EXIT_CODES databaseRead(FILE *database, tree_t *tree);
static EXIT_CODES databaseReadNode(FILE *database, treeNode_t **node);

EXIT_CODES treeNodeInit(treeNode_t **node, treeNodeElem_t *value)
{
    // Initialization
    *node = (treeNode_t *) calloc(1, sizeof(treeNode_t));
    CHECK_CALLOC_RESULT(node);
    
    strcpy((*node)->value, value);
    (*node)->left  = NULL;
    (*node)->right = NULL;

    return EXIT_CODES::NO_ERRORS;
}

EXIT_CODES treeCtorFromDatabase(tree_t *tree, char *databaseFileName)
{
    // Error check
    if (tree == NULL || databaseFileName == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::PASSED_OBJECT_IS_NULLPTR);
        return EXIT_CODES::PASSED_OBJECT_IS_NULLPTR;
    }

    // Tree construction
    FILE *database = fopen(databaseFileName, "r");
    CHECK_FOPEN_RESULT(database);

    // Construct tree from `database` data
    IS_ERROR(databaseRead(database, tree))
    {
        PRINT_ERROR_TRACING_MESSAGE(TREE_EXIT_CODES::ERROR_DURING_TREE_CONSTRUCTION_FROM_DATABASE);
        return EXIT_CODES::BAD_FUNC_RESULT;
    }

    // Destruction
    fclose(database);

    return EXIT_CODES::NO_ERRORS;
}

static EXIT_CODES databaseRead(FILE *database, tree_t *tree)
{
    // Error check
    if (database == NULL || tree == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::PASSED_OBJECT_IS_NULLPTR);
        return EXIT_CODES::PASSED_OBJECT_IS_NULLPTR;
    }

    // Read database
    IS_ERROR(databaseReadNode(database, &tree->root))
    {
        PRINT_ERROR_TRACING_MESSAGE(TREE_DATABASE_EXIT_CODES::ERROR_DURING_READING_DATABASE_TREE_NODE);
        return EXIT_CODES::BAD_FUNC_RESULT;
    }

    return EXIT_CODES::NO_ERRORS;
}

static EXIT_CODES databaseReadNode(FILE *database, treeNode_t **node)
{
    // Error check
    if (database == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::PASSED_OBJECT_IS_NULLPTR);
        return EXIT_CODES::PASSED_OBJECT_IS_NULLPTR;
    }

    // Read node
    char treeNodeValue[MAX_TREE_NODE_VALUE_STR_LENGTH] = {};
    int databaseCharsRead = 0;
    fscanf(database, " { \"%" TO_STR(MAX_TREE_NODE_VALUE_STR_LENGTH) "[^\"]\"%n ", treeNodeValue, &databaseCharsRead);

    // Check `databaseCharsRead` for format && errors
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
    *node = (treeNode_t *) calloc(1, sizeof(treeNode_t));
    CHECK_CALLOC_RESULT(node);

    // Init new node `value` field
    strcpy((*node)->value, treeNodeValue);

    // Init new node branches (`left` && `right` fields)
    (*node)->left = NULL;
    (*node)->right = NULL;

    // Fill other nodes
    char bracket[2] = {};
    fscanf(database, BRACKET_REGEXP, bracket);
    if (bracket[0] == '{')
    {
        // Move stream pointer to open bracket ('{') position in file stream
        int ret = ungetc(bracket[0], database);
        CHECK_UNGETC_RESULT(ret);

        // Fill left child
        IS_ERROR(databaseReadNode(database, &((*node)->left)))
        {
            PRINT_ERROR_TRACING_MESSAGE(TREE_DATABASE_EXIT_CODES::ERROR_DURING_READING_DATABASE_LEFT_NODE);
            return EXIT_CODES::BAD_FUNC_RESULT;
        }

        // Fill right child
        IS_ERROR(databaseReadNode(database, &((*node)->right)))
        {
            PRINT_ERROR_TRACING_MESSAGE(TREE_DATABASE_EXIT_CODES::ERROR_DURING_READING_DATABASE_RIGHT_NODE);
            return EXIT_CODES::BAD_FUNC_RESULT;
        }

        // Check database for correct format
        memset(bracket, 0, sizeof(bracket));
        fscanf(database, BRACKET_REGEXP, bracket);
        if (bracket[0] != '}')
        {
            PRINT_ERROR_TRACING_MESSAGE(TREE_DATABASE_EXIT_CODES::BAD_DATABASE_FORMAT);
            return EXIT_CODES::BAD_OBJECT_PASSED;
        }
    }

    return EXIT_CODES::NO_ERRORS;
}

EXIT_CODES treeDFSWActionFunction(tree_t *tree, actionFunction func, void *arg)
{
    // Error check
    if (tree == NULL || func == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::PASSED_OBJECT_IS_NULLPTR);
        return EXIT_CODES::PASSED_OBJECT_IS_NULLPTR;
    }

    // Main DFS
    stack_t stack = {};
    IS_ERROR(stackCtor(&stack))
    {
        PRINT_ERROR_TRACING_MESSAGE(STACK_EXIT_CODES::ERROR_DURING_STACK_CTOR);
        return EXIT_CODES::BAD_FUNC_RESULT;
    }

    IS_ERROR(stackPush(&stack, tree->root))
    {
        PRINT_ERROR_TRACING_MESSAGE(STACK_EXIT_CODES::ERROR_DURING_STACK_PUSH);
        return EXIT_CODES::BAD_FUNC_RESULT;
    }

    while (stack.size > 0)
    {
        // Get each new node from stack (queue)
        treeNode_t *node = {};
        IS_ERROR(stackPop(&stack, &node))
        {
            PRINT_ERROR_TRACING_MESSAGE(STACK_EXIT_CODES::ERROR_DURING_STACK_POP);
            return EXIT_CODES::BAD_FUNC_RESULT;
        }

        // Push each child of a node
        int nodeBranches = func(node, arg);
        if (nodeBranches & NODE_HAS_LEFT_BRANCH)
        {
            IS_ERROR(stackPush(&stack, node->left))
            {
                PRINT_ERROR_TRACING_MESSAGE(TREE_EXIT_CODES::FAIL_TO_PUSH_NODES_LEFT_CHILD);
                return EXIT_CODES::BAD_FUNC_RESULT;
            }
        }

        if (nodeBranches & NODE_HAS_RIGHT_BRANCH)
        {
            IS_ERROR(stackPush(&stack, node->right))
            {
                PRINT_ERROR_TRACING_MESSAGE(TREE_EXIT_CODES::FAIL_TO_PUSH_NODES_RIGHT_CHILD);
                return EXIT_CODES::BAD_FUNC_RESULT;
            }
        }
    }

    // Destruction
    IS_ERROR(stackDtor(&stack))
    {
        PRINT_ERROR_TRACING_MESSAGE(STACK_EXIT_CODES::ERROR_DURING_STACK_DTOR);
        return EXIT_CODES::BAD_FUNC_RESULT;
    }

    return EXIT_CODES::NO_ERRORS;
}

static EXIT_CODES nodeExportToDatabase(treeNode_t *node, FILE *database)
{
    // Error check
    if (node == NULL || database == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::PASSED_OBJECT_IS_NULLPTR);
        return EXIT_CODES::PASSED_OBJECT_IS_NULLPTR;
    }

    // Database begin
    fprintf(database, "{\"%s\"", node->value);

    // Export left branch
    if (node->left != NULL)
    {
        fputc(' ', database);

        nodeExportToDatabase(node->left, database);
    }
    
    // Export right branch
    if (node->right != NULL)
    {
        fputc(' ', database);

        nodeExportToDatabase(node->right, database);
    }

    // Database end
    fputc('}', database);

    return EXIT_CODES::NO_ERRORS;
}

EXIT_CODES treeExportToDatabase(tree_t *tree, char *databaseFileName)
{
    // Error check
    if (tree == NULL || databaseFileName == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::PASSED_OBJECT_IS_NULLPTR);
        return EXIT_CODES::PASSED_OBJECT_IS_NULLPTR;
    }

    // Open database
    FILE *database = fopen(databaseFileName, "w");
    CHECK_FOPEN_RESULT(database);

    // Export
    IS_ERROR(nodeExportToDatabase(tree->root, database))
    {
        // Clean
        fclose(database);

        PRINT_ERROR_TRACING_MESSAGE(TREE_DATABASE_EXIT_CODES::ERROR_DURING_EXPORTING_TREE_TO_DATABASE);
        return EXIT_CODES::BAD_FUNC_RESULT;
    }

    // Destruction
    fclose(database);

    return EXIT_CODES::NO_ERRORS;
}

// TODO: implement BFS using list (pushBack or append)
int treeBFSSearch(tree_t *tree, treeNodeElem_t *searchValue)
{
    // Error check
    assert(tree != NULL && searchValue != NULL && "EXIT_CODES::PASSED_OBJECT_IS_NULLPTR");

    // Main BFS algorithm

}
