#include <stdlib.h>  // for calloc && realloc && free
#include <stdio.h>

#include "../include/list.h"
#include "../../colors/colors.h"

bool listOK(list_t *list)
{
    return  list != NULL && list->nodes != NULL && \
            list->size >= 0 && list->size <= list->capacity;
}

EXIT_CODES listCtor(list_t *list, int listCapacity)
{
    // Error check
    if (list == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::PASSED_OBJECT_IS_NULLPTR);
        return EXIT_CODES::PASSED_OBJECT_IS_NULLPTR;
    }

    if (list->head != POISON || list->size != POISON)// || list->tail != POISON)
    {
        PRINT_ERROR_TRACING_MESSAGE(LIST_EXIT_CODES::OLD_LIST_PASSED);
        return EXIT_CODES::BAD_OBJECT_PASSED;
    }

    if (listCapacity < 0)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::BAD_OBJECT_PASSED);
        return EXIT_CODES::BAD_OBJECT_PASSED;
    }

    // Allocate some nodes
    node_t *nodes = (node_t *) calloc(listCapacity, sizeof(node_t));
    CHECK_CALLOC_RESULT(nodes);

    // Init fields
    list->nodes     = nodes;
    list->capacity  = listCapacity;
    list->nodes[0]  = {0, POISON, 0};        // setup head node
    for (int node = 1; node < list->capacity; ++node)
    {
        // list->nodes[node].prev = node - 1; FIXME:
        list->nodes[node].prev = POISON;
        list->nodes[node].data = POISON;
        list->nodes[node].next = node + 1;
    }
    list->nodes[list->capacity - 1].next = 0;

    list->size      = 0;
    list->head      = 0;
    list->freeNode  = 1;
    
    return EXIT_CODES::NO_ERRORS;
}

EXIT_CODES listDtor(list_t *list)
{
    // Error check
    OBJECT_VERIFY(list, list);

    // Destruction
    free(list->nodes);

    // Poison list fields
    list->nodes     = NULL;
    list->head      = POISON;
    list->freeNode  = POISON;
    list->capacity  = POISON;
    list->size      = POISON;
    
    return EXIT_CODES::NO_ERRORS;
}

EXIT_CODES listGetNextNode(list_t *list, index_t index, index_t *result)
{
    // Error check
    OBJECT_VERIFY(list, list);
    if (index >= list->capacity || index < 0)
    {
        PRINT_ERROR_TRACING_MESSAGE(LIST_EXIT_CODES::BAD_LIST_INDEX_PASSED);
        return EXIT_CODES::BAD_OBJECT_PASSED;
    }

    *result = list->nodes[index].next;

    return EXIT_CODES::NO_ERRORS;
}

EXIT_CODES listGetPrevNode(list_t *list, index_t index, index_t *result)
{
    // Error check
    OBJECT_VERIFY(list, list);
    if (index >= list->capacity || index < 0)
    {
        PRINT_ERROR_TRACING_MESSAGE(LIST_EXIT_CODES::BAD_LIST_INDEX_PASSED);
        return EXIT_CODES::BAD_OBJECT_PASSED;
    }

    *result = list->nodes[index].prev;

    return EXIT_CODES::NO_ERRORS;
}

EXIT_CODES listInsertAfter(list_t *list, index_t index, nodeDataType_t value)
{
    // Error check
    OBJECT_VERIFY(list, list);
    if (index < 0 || index >= list->capacity)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::BAD_OBJECT_PASSED);
        return EXIT_CODES::BAD_OBJECT_PASSED;
    }

    // Check for reallocation
    if (list->size == list->capacity - 1)
    {
        IS_ERROR(listCapacityIncrease(list))
        {
            PRINT_ERROR_TRACING_MESSAGE(LIST_EXIT_CODES::BAD_LIST_CAPACITY_INCREASE_FUNC_RESULT);
            return EXIT_CODES::BAD_FUNC_RESULT;
        }
    }

    // Get insertion index
    int insertIndex = list->freeNode;
    list->freeNode  = list->nodes[list->freeNode].next;

    // Connect inserted node with surrounded
    list->nodes[insertIndex].prev   = index;
    list->nodes[insertIndex].data   = value;
    list->nodes[insertIndex].next   = list->nodes[index].next;

    // Connect surrounded nodes with inserted node
    list->nodes[list->nodes[index].next].prev   = insertIndex;
    list->nodes[index].next                     = insertIndex;

    list->size++;

    if (list->freeNode == 0)
    {
        list->freeNode = list->size + 1;
    }

    return EXIT_CODES::NO_ERRORS;
}

EXIT_CODES listAppend(list_t *list, nodeDataType_t value)
{
    // Error check
    OBJECT_VERIFY(list, list);

    // Append
    printf("list->size: %d\n", list->size);
    printf("list->nodes[list->head].prev: %d\n", list->nodes[list->head].prev);
    IS_ERROR(listInsertAfter(list, list->nodes[list->head].prev, value))
    {
        PRINT_ERROR_TRACING_MESSAGE(LIST_EXIT_CODES::BAD_LIST_INSERT_AFTER_RESULT_IN_APPEND);
        return EXIT_CODES::BAD_FUNC_RESULT;
    }

    return EXIT_CODES::NO_ERRORS;
}

EXIT_CODES listRemove(list_t *list, index_t index)
{
    // Error check
    OBJECT_VERIFY(list, list);
    if (index < 0 || index >= list->capacity)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::BAD_OBJECT_PASSED);
        return EXIT_CODES::BAD_OBJECT_PASSED;
    }

    // Relink surrounding nodes
    list->nodes[list->nodes[index].next].prev = list->nodes[index].prev;
    list->nodes[list->nodes[index].prev].next = list->nodes[index].next;

    // Delete `index` node (poison it)
    list->nodes[index].prev = POISON;
    list->nodes[index].data = POISON;
    list->nodes[index].next = list->freeNode;
    list->freeNode = index;

    list->size--;

    return EXIT_CODES::NO_ERRORS;
}

EXIT_CODES listCapacityIncrease(list_t *list)
{
    // Error check
    OBJECT_VERIFY(list, list);
    
    // Capacity increase
    int newCapacity = list->capacity * CAPACITY_INCREASE_COEFF;
    node_t *new_nodes = (node_t *) realloc(list->nodes, sizeof(node_t) * newCapacity);
    CHECK_REALLOC_RESULT(new_nodes);

    // Update nodes
    list->nodes = new_nodes;

    // Link new allocated nodes
    for (int node = list->capacity; node < newCapacity; ++node)
    {
        // list->nodes[node].prev = node - 1;
        list->nodes[node].prev = POISON;
        list->nodes[node].data = POISON;
        list->nodes[node].next = node + 1;
    }
    list->nodes[newCapacity - 1].next = 0;

    // Update capacity
    list->capacity  = newCapacity;

    return EXIT_CODES::NO_ERRORS;
}

EXIT_CODES listDump(list_t *list)
{
    // Error check
    if (list == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::PASSED_OBJECT_IS_NULLPTR);
        return EXIT_CODES::PASSED_OBJECT_IS_NULLPTR;
    }

    printf("list->head: %d\n", list->head);
    printf("list->freeNode: %d\n", list->freeNode);
    printf("list->size: %d\n", list->size);
    printf("list->capacity: %d\n", list->capacity);

    // Dump
    for (int node = 0; node < list->size + 1; ++node)
    {
        printf(YELLOW "[%d]:\n" RESET, node);
        printf("\tlist->nodes[%d].prevIndx: " YELLOW "%d" RESET " (%d)\n", node, list->nodes[node].prev, list->nodes[list->nodes[node].prev].data);
        printf("\tlist->nodes[%d].data: %d\n", node, list->nodes[node].data);
        printf("\tlist->nodes[%d].nextIndx: " YELLOW "%d" RESET " (%d)\n", node, list->nodes[node].next, list->nodes[list->nodes[node].next].data);
    }
    putchar('\n');

    return EXIT_CODES::NO_ERRORS;
}

EXIT_CODES listCreateGraph(list_t *list)
{
    // Error check
    OBJECT_VERIFY(list, list);

    printf("list->freenode: %d\n", list->freeNode);

    // Output file
    FILE *fs = fopen("out.dot", "w");
    CHECK_FOPEN_RESULT(fs);

    // Init nodes
    fprintf(fs, "digraph List {\n");
    fprintf(fs, "\tNode0 [shape=\"record\", label=\"<np0> %d | {0 | %d} | <nn0> %d\", style=filled, fillcolor=orange];",
                list->nodes[0].prev, list->nodes[0].data, list->nodes[0].next);

    for (int node = 1; node < list->capacity; ++node)
    {
        fprintf(fs, "\tNode%d [shape=\"record\", label=\"<np%d> %d | {%d | %d} | <nn%d> %d\"];\n",
                    node, node, list->nodes[node].prev, node, list->nodes[node].data, node, list->nodes[node].next);
    }

    // Place nodes
    for (int node = 0; node < list->capacity - 1; ++node)
    {
        fprintf(fs, "\tNode%d->Node%d[style=invis];\n", node, node + 1);
    }

    // Connect nodes using prev and next
    for (int node = 0; node < list->capacity; ++node)
    {
        fprintf(fs, "\tNode%d:nn%d -> Node%d[color=\"%s\"];\n",
                    node, node, list->nodes[node].next,
                    (list->nodes[node].prev == POISON ? "black" : "chartreuse"));
        
        if (list->nodes[node].prev != POISON)
        {
            fprintf(fs, "\tNode%d:np%d -> Node%d[color=\"red\"];\n",
                        node, node, list->nodes[node].prev);
        }
    }

    fprintf(fs, "\tfree[shape=\"record\", style=filled, fillcolor=azure3];\n");
    fprintf(fs, "\tfree -> Node%d;\n", list->freeNode);
    fprintf(fs, "}\n");

    fclose(fs);

    system(CREATE_GRAPH_COMMAND);

    return EXIT_CODES::NO_ERRORS;
}
