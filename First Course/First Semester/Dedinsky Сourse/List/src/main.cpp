#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../include/list.h"

int main()
{
    srand(time(NULL));

    list_t list = {};
    IS_ERROR(listCtor(&list, 4))
    {
        EXIT_WITH_TRACE(EXIT_CODES::CONSTRUCTOR_ERROR, EXIT_FAILURE);
    }

    for (int i = 0; i < 8; ++i)
    {
        listInsertAfter(&list, i, i + 1);
    }

    listRemove(&list, 2);
    // listInsertAfter(&list, 1, 111);
    // listRemove(&list, 6);
    // listInsertAfter(&list, 5, 333);
    listAppend(&list, 999);
    // listAppend(&list, 1423);
    listCreateGraph(&list); 


    IS_ERROR(listDtor(&list))
    {
        EXIT_WITH_TRACE(EXIT_CODES::DESTRUCTOR_ERROR, EXIT_FAILURE);
    }

    return 0;
}
