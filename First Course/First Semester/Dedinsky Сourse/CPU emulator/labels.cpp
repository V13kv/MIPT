#include <stdbool.h>
#include <stdlib.h>  // for calloc && free && realloc

#include "include/asm/labels.h"

EXIT_CODES labelsCtor(labels_t *labels)
{
    // Error check
    if (labels == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::PASSED_OBJECT_IS_NULLPTR);
        return EXIT_CODES::PASSED_OBJECT_IS_NULLPTR;
    }

    // Construction
    labels->labels = (label_t *) calloc(MIN_LABELS_COUNT, sizeof(label_t));
    if (labels->labels == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::BAD_STD_FUNC_RESULT);
        return EXIT_CODES::BAD_STD_FUNC_RESULT;
    }

    labels->currAllocatedLabels = MIN_LABELS_COUNT;

    return EXIT_CODES::NO_ERRORS;
}

EXIT_CODES labelsDtor(labels_t *labels)
{
    // Error check
    if (labels == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::PASSED_OBJECT_IS_NULLPTR);
        return EXIT_CODES::PASSED_OBJECT_IS_NULLPTR;
    }

    // Destruction
    free(labels->labels);
    labels->labels                  = NULL;
    labels->currAllocatedLabels     = 0;
    labels->globalOffset            = 0;
    labels->totalLabels             = 0;

    return EXIT_CODES::NO_ERRORS;
}

EXIT_CODES expandLabelsArray(labels_t *labels)
{
    // Error check
    if (labels == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::PASSED_OBJECT_IS_NULLPTR);
        return EXIT_CODES::PASSED_OBJECT_IS_NULLPTR;
    }

    // Reallocation
    label_t *newLabels = (label_t *) realloc(labels->labels, ALLOC_INC_COEF * labels->currAllocatedLabels * sizeof(label_t));
    if (newLabels == NULL)
    {
        IS_OK_W_EXIT(labelsDtor(labels));

        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::BAD_STD_FUNC_RESULT);
        return EXIT_CODES::BAD_STD_FUNC_RESULT;
    }
    labels->currAllocatedLabels = ALLOC_INC_COEF * labels->currAllocatedLabels;

    return EXIT_CODES::NO_ERRORS;
}

// TODO: label strip function
EXIT_CODES initLabel(text_line_t *line, labels_t *labels)
{
    // Error check
    if (line == NULL || labels == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::PASSED_OBJECT_IS_NULLPTR);
        return EXIT_CODES::PASSED_OBJECT_IS_NULLPTR;
    }

    // Initialization
    label_t *currentLabel = &labels->labels[labels->totalLabels];
    int ret = sscanf(line->beginning, LABEL_FORMAT, currentLabel->name, &currentLabel->length);
    if (ret == 0 || ret == EOF)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::BAD_STD_FUNC_RESULT);
        return EXIT_CODES::BAD_STD_FUNC_RESULT;
    }
    labels->totalLabels++;

    if (labels->totalLabels >= labels->currAllocatedLabels)
    {
        IS_OK_W_EXIT(expandLabelsArray(labels));
    }

    return EXIT_CODES::NO_ERRORS;
}

bool isLabel(text_line_t *line)
{
    // Error check
    if (line == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::PASSED_OBJECT_IS_NULLPTR);
        exit(EXIT_FAILURE);
    }

    // Check
    label_t temp = {};
    int ret = sscanf(line->beginning, LABEL_FORMAT, temp.name, &temp.length);
    if (ret == 0 || ret == EOF)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::BAD_STD_FUNC_RESULT);
        exit(EXIT_FAILURE);
    }

    if (temp.length == 0)
    {
        return false;
    }

    return true;
}
