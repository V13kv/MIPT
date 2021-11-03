#include <stdbool.h>
#include <stdlib.h>  // for calloc && free && realloc

#include "../../include/asm/assembler.h"
#include "../../include/asm/labels.h"

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

    labels->labels = newLabels;
    labels->currAllocatedLabels = ALLOC_INC_COEF * labels->currAllocatedLabels;

    return EXIT_CODES::NO_ERRORS;
}

// TODO: label strip function
EXIT_CODES initLabel(char *data, labels_t *labels, const char *LABEL_FORMAT)
{
    // Error check
    if (data == NULL || labels == NULL || LABEL_FORMAT == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::PASSED_OBJECT_IS_NULLPTR);
        return EXIT_CODES::PASSED_OBJECT_IS_NULLPTR;
    }

    // Initialization
    label_t *currentLabel = &labels->labels[labels->totalLabels];
    int ret = sscanf(data, LABEL_FORMAT, currentLabel->name, &currentLabel->length);

    // Error check + format check
    CHECK_SSCANF_RESULT(ret);
    if (ret != 1)
    {
        PRINT_ERROR_TRACING_MESSAGE(ASM_EXIT_CODES::BAD_LABEL_NAME);
        return EXIT_CODES::BAD_OBJECT_PASSED;
    }

    // Update `labels` fiels
    currentLabel->offset = globalOffset; // currentLabel->offset = labels->globalOffset;
    ++labels->totalLabels;

    // printf("labels[%d].name: %s\n", labels->totalLabels - 1, labels->labels[labels->totalLabels - 1].name);

    // Check for reallocation
    if (labels->totalLabels >= labels->currAllocatedLabels)
    {
        // printf("\nReallocation!\n");
        IS_OK_W_EXIT(expandLabelsArray(labels));
    }

    return EXIT_CODES::NO_ERRORS;
}

bool isLabel(char *data, const char *LABEL_FORMAT)
{
    // Error check
    if (data == NULL || LABEL_FORMAT == NULL)
    {
        PRINT_ERROR_TRACING_MESSAGE(EXIT_CODES::PASSED_OBJECT_IS_NULLPTR);
        exit(EXIT_FAILURE);
    }

    // Check
    label_t temp = {};
    int ret = sscanf(data, LABEL_FORMAT, temp.name, &temp.length);
    if (ret != 1 || ret == EOF)
    {
        PRINT_ERROR_TRACING_MESSAGE(ASM_EXIT_CODES::BAD_LABEL_FORMAT);
        exit(EXIT_FAILURE);
    }

    if (temp.length == 0)
    {
        return false;
    }

    return true;
}
