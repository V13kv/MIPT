#ifndef LABELS_H
#define LABELS_H

#include <stdbool.h>

#define DEBUG_LEVEL 1
#include "../../lib/debug/debug.h"

#define LABEL_LINE_FORMAT               "%[a-zA-Z0-9_]:%lln"
#define LABEL_ARG_FORMAT                "%[a-zA-Z0-9_]%lln"
const int MIN_LABELS_COUNT              = 2;
const int ALLOC_INC_COEF                = 2;
const int MAX_LABEL_STR_LENGTH          = 50;

enum class LABELS_EXIT_CODES
{
    BAD_LABEL_NAME,
    BAD_LABEL_FORMAT,
};

typedef long long int ll;

struct label_t
{
    char name[MAX_LABEL_STR_LENGTH] = {};
    ll length                       = 0;
    long int offset                 = 0;
};

struct labels_t
{
    label_t *labels         = NULL;
    ll currAllocatedLabels  = 0;
    int totalLabels         = 0;
};

EXIT_CODES labelsCtor(labels_t *labels);
EXIT_CODES labelsDtor(labels_t *labels);

bool isLabel(char *data, const char *LABEL_FORMAT);
EXIT_CODES initLabel(char *data, labels_t *labels, const char *LABEL_FORMAT, const int globalOffset);

EXIT_CODES expandLabelsArray(labels_t *labels);


#endif  // LABELS_H
