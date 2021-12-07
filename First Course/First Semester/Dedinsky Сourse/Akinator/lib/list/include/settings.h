#ifndef LIST_SETTINGS_H
#define LIST_SETTINGS_H

#include "../../../include/tree.h"

#define nodeDataType_t              treeNode_t*  //int
#define CREATE_GRAPH_COMMAND        "dot.exe -Tsvg out.dot -o graph.svg"

#ifndef POISON
    const int POISON                    = -663;
#endif
const int MIN_LIST_NODES            = 8;
const int CAPACITY_INCREASE_COEFF   = 2;


#endif  // LIST_SETTINGS_H
