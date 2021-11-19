#ifndef LIST_H
#define LIST_H


#define DEBUG_LEVEL 1
#include "../lib/debug/debug.h"
#include "settings.h"

typedef int index_t;

enum class LIST_EXIT_CODES
{
    OLD_LIST_PASSED,
    BAD_LIST_CAPACITY_INCREASE_FUNC_RESULT,
    BAD_LIST_INDEX_PASSED,
    BAD_LIST_INSERT_AFTER_RESULT_IN_APPEND,
};

struct node_t
{
    index_t prev        = POISON;
    nodeDataType_t data = POISON;
    index_t next        = POISON;
};

struct list_t
{
    node_t *nodes       = {};
    index_t head        = POISON;  // Pros && cons of node_t head
    index_t freeNode    = POISON;
    int size            = POISON;
    int capacity        = POISON;
};

bool listOK(list_t *list);  // DONE
EXIT_CODES listCtor(list_t *list, int listCapacity = MIN_LIST_NODES);  // DONE
EXIT_CODES listDtor(list_t *list);  // DONE
EXIT_CODES listCapacityIncrease(list_t *list);  // DONE

EXIT_CODES listGetNextNode(list_t *list, index_t index, index_t *result);  // DONE
EXIT_CODES listGetPrevNode(list_t *list, index_t index, index_t *result);  // DONE
EXIT_CODES listInsertAfter(list_t *list, index_t index, nodeDataType_t value);  // DONE
EXIT_CODES listAppend(list_t *list, nodeDataType_t value);
EXIT_CODES listRemove(list_t *list, index_t index);  // DONE

EXIT_CODES listDump(list_t *list);  // DONE
EXIT_CODES listCreateGraph(list_t *list);  // DONE



#endif  // LIST_H
