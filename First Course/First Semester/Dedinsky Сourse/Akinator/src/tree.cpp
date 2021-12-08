#include <stdio.h>
#include <string.h>  // for strcpy

#include <assert.h>  // for assert

#define DEBUG_LEVEL 1
#include "../lib/debug/debug.h"

#include "../include/tree.h"
#include "../lib/stack/include/stack.h"
#include "../lib/list/include/list.h"
#include "../include/settings.h"

static EXIT_CODES databaseRead(FILE *database, tree_t *tree);
static EXIT_CODES databaseReadNode(FILE *database, treeNode_t **node);

EXIT_CODES treeNodeInit(treeNode_t **node, treeNodeElem_t *value)
{
    // Initialization
    *node = (treeNode_t *) calloc(1, sizeof(treeNode_t));
    CHECK_CALLOC_RESULT(node);
    
    if (value != NULL)
    {
        strcpy((*node)->value, value);
    }
    (*node)->left  = NULL;
    (*node)->right = NULL;

    return EXIT_CODES::NO_ERRORS;
}

EXIT_CODES treeNodeDeinit(treeNode_t **node)
{
    // Error check
    if (node == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::PASSED_OBJECT_IS_NULLPTR);
        return EXIT_CODES::PASSED_OBJECT_IS_NULLPTR;
    }

    // Deinitialization
    free(*node);
    *node = NULL;

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
        PRINT_ERROR_TRACING_MESSAGE(TREE_EXIT_CODES::TREE_CTOR_ERROR);
        return EXIT_CODES::BAD_FUNC_RESULT;
    }

    // Destruction
    fclose(database);

    return EXIT_CODES::NO_ERRORS;
}

EXIT_CODES treeDtor(tree_t *tree)
{
    // Error check
    if (tree == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::PASSED_OBJECT_IS_NULLPTR);
        return EXIT_CODES::PASSED_OBJECT_IS_NULLPTR;
    }

    // Destruction
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

    // Destruction of all nodes
    treeNode_t *node = NULL;
    while (stack.size > 0)
    {
        // Get each node
        IS_ERROR(stackPop(&stack, &node))
        {
            PRINT_ERROR_TRACING_MESSAGE(STACK_EXIT_CODES::ERROR_DURING_STACK_POP);
            return EXIT_CODES::BAD_FUNC_RESULT;
        }

        // Push its left child
        if (node->left != NULL)
        {
            IS_ERROR(stackPush(&stack, node->left))
            {
                PRINT_ERROR_TRACING_MESSAGE(TREE_EXIT_CODES::FAIL_TO_PUSH_NODES_LEFT_CHILD);
                return EXIT_CODES::BAD_FUNC_RESULT;
            }
        }

        // Push its right child
        if (node->right != NULL)
        {
            IS_ERROR(stackPush(&stack, node->right))
            {
                PRINT_ERROR_TRACING_MESSAGE(TREE_EXIT_CODES::FAIL_TO_PUSH_NODES_RIGHT_CHILD);
                return EXIT_CODES::BAD_FUNC_RESULT;
            }
        }

        // Destruct node
        IS_ERROR(treeNodeDeinit(&node))
        {
            PRINT_ERROR_TRACING_MESSAGE(TREE_EXIT_CODES::TREE_NODE_DEINIT_ERROR);
            return EXIT_CODES::BAD_FUNC_RESULT;
        }
    }

    // Cleaning
    IS_ERROR(stackDtor(&stack))
    {
        PRINT_ERROR_TRACING_MESSAGE(STACK_EXIT_CODES::ERROR_DURING_STACK_DTOR);
        return EXIT_CODES::DESTRUCTOR_ERROR;
    }

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
    char treeNodeValue[MAX_TREE_NODE_VALUE_STR_LENGTH + 1] = {};
    int databaseCharsRead = 0;
    fscanf(database, " { \"%" TO_STR(MAX_TREE_NODE_VALUE_STR_LENGTH) "[^\"]\"%n", treeNodeValue, &databaseCharsRead);

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

    // Init new node
    IS_ERROR(treeNodeInit(node, treeNodeValue))
    {
        PRINT_ERROR_TRACING_MESSAGE(TREE_EXIT_CODES::TREE_NODE_INIT_ERROR);
        return EXIT_CODES::BAD_FUNC_RESULT;
    }

    // Fill other nodes
    char bracket[2] = {};
    fscanf(database, BOTH_BRACKETS_REGEXP, bracket);
    if (bracket[0] == '{')
    {
        // Move stream pointer to open bracket ('{') position in file stream
        int ret = ungetc(bracket[0], database);
        CHECK_UNGETC_RESULT(ret);

        // Fill left child
        IS_ERROR(databaseReadNode(database, &(*node)->left))
        {
            PRINT_ERROR_TRACING_MESSAGE(TREE_DATABASE_EXIT_CODES::ERROR_DURING_READING_DATABASE_LEFT_NODE);
            return EXIT_CODES::BAD_FUNC_RESULT;
        }

        // Check for right child
        memset(bracket, 0, sizeof(bracket));
        fscanf(database, OPEN_BRACKET_REGEXP, bracket);
        if (bracket[0] == '{')
        {
            // Move stream pointer to open bracket ('{') position in file stream
            CHECK_UNGETC_RESULT(ungetc(bracket[0], database));      

            // Fill right child
            IS_ERROR(databaseReadNode(database, &(*node)->right))
            {
                PRINT_ERROR_TRACING_MESSAGE(TREE_DATABASE_EXIT_CODES::ERROR_DURING_READING_DATABASE_RIGHT_NODE);
                return EXIT_CODES::BAD_FUNC_RESULT;
            }
        }
        
        // Check database for correct format
        memset(bracket, 0, sizeof(bracket));
        fscanf(database, CLOSE_BRACKET_REGEXP, bracket);
        if (bracket[0] != '}')
        {
            PRINT_ERROR_TRACING_MESSAGE(TREE_DATABASE_EXIT_CODES::CLOSE_BRACKET_NOT_FOUND);
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

    treeNode_t *node = NULL;
    while (stack.size > 0)
    {
        // Get each new node from stack (queue)
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
        return EXIT_CODES::DESTRUCTOR_ERROR;
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

EXIT_CODES treeBFSSearch(const tree_t *const tree, treeNodeElem_t *searchValue)
{
    // Error check
    if (tree == NULL || searchValue == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::PASSED_OBJECT_IS_NULLPTR);
        return EXIT_CODES::PASSED_OBJECT_IS_NULLPTR;
    }

    // Using list as a queue
    list_t list = {};
    IS_ERROR(listCtor(&list))
    {
        PRINT_ERROR_TRACING_MESSAGE(LIST_EXIT_CODES::LIST_CONSTRUCTION_ERROR);
        return EXIT_CODES::CONSTRUCTOR_ERROR;
    }

    IS_ERROR(listAppend(&list, tree->root))
    {
        PRINT_ERROR_TRACING_MESSAGE(LIST_EXIT_CODES::LIST_APPEND_ERROR);
        return EXIT_CODES::BAD_FUNC_RESULT;
    }
    
    // Main BFS algorithm
    treeNode_t *node = NULL;
    index_t nodeIndex = 0;
    bool searchNodeFound = false;
    while (list.size > 0 && !searchNodeFound)
    {
        // Extract each node
        IS_ERROR(listGetNextNode(&list, list.head, &nodeIndex))
        {
            PRINT_ERROR_TRACING_MESSAGE(LIST_EXIT_CODES::LIST_GET_NEXT_NODE_ERROR);
            return EXIT_CODES::BAD_FUNC_RESULT;
        }

        node = list.nodes[nodeIndex].data;

        IS_ERROR(listRemove(&list, nodeIndex))
        {
            PRINT_ERROR_TRACING_MESSAGE(LIST_EXIT_CODES::LIST_REMOVE_ERROR);
            return EXIT_CODES::BAD_FUNC_RESULT;
        }

        // Check for search value
        if (!strcmp(node->value, searchValue))
        {
            searchNodeFound = true;
        }
        else
        {
            // Append its neighbours
            if (node->left != NULL)
            {
                IS_ERROR(listAppend(&list, node->left))
                {
                    PRINT_ERROR_TRACING_MESSAGE(LIST_EXIT_CODES::LIST_APPEND_ERROR);
                    return EXIT_CODES::BAD_FUNC_RESULT;
                }
            }

            if (node->right != NULL)
            {
                IS_ERROR(listAppend(&list, node->right))
                {
                    PRINT_ERROR_TRACING_MESSAGE(LIST_EXIT_CODES::LIST_APPEND_ERROR);
                    return EXIT_CODES::BAD_FUNC_RESULT;
                }    
            }
        }   
    }

    if (searchNodeFound)
    {
        printf(TURN_TO_GREEN(VALUE_FOUND_MESSAGE));
        printf(TURN_TO_YELLOW(NODE_INFO_MESSAGE), (void *) node);
        printf(NODE_VALUE_MESSAGE, node->value);

        if (node->left != NULL)
        {
            printf(NODE_LEFT_BRANCH_MESSAGE, node->left->value, (void *) node->left);
        }

        if (node->right != NULL)
        {
            printf(NODE_RIGHT_BRANCH_MESSAGE, node->right->value, (void *) node->right);
        }
    }
    else
    {
        printf(TURN_TO_RED(VALUE_NOT_FOUND_MESSAGE));
    }

    IS_ERROR(listDtor(&list))
    {
        PRINT_ERROR_TRACING_MESSAGE(LIST_EXIT_CODES::LIST_DESTRUCTION_ERROR);
        return EXIT_CODES::DESTRUCTOR_ERROR;
    }

    return EXIT_CODES::NO_ERRORS;
}
