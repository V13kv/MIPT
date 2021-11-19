#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "../include/list.h"

// TODO: get rid of copypaste
int main()
{
    srand(time(NULL));

    list_t list = {};
    IS_ERROR(listCtor(&list, 4))
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::CONSTRUCTOR_ERROR);
        listDump(&list);
        return EXIT_FAILURE;
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
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::DESTRUCTOR_ERROR);
        listDump(&list);
        return EXIT_FAILURE;
    }

    return 0;
}
