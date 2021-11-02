#ifndef LABELS_H
#define LABELS_H

#include <stdbool.h>

#define DEBUG_LEVEL 1
#include "../../lib/debug/debug.h"
#include "../../lib/text/include/text.h"

typedef long long int ll;

#define LABEL_FORMAT                    "%[a-zA-Z0-9_]:%lln"
const int MIN_LABELS_COUNT              = 2;
const int ALLOC_INC_COEF                = 2;
const int MAX_LABEL_STR_LENGTH          = 50;

struct label_t
{
    char name[MAX_LABEL_STR_LENGTH] = {};
    ll length                       = 0;
    ll offset                       = 0;
};

struct labels_t
{
    label_t *labels             = NULL;
    ll currAllocatedLabels      = 0;
    ll totalLabels              = 0;
    ll globalOffset             = 0;
};


EXIT_CODES labelsCtor(labels_t *labels);
EXIT_CODES labelsDtor(labels_t *labels);

bool isLabel(text_line_t *line);
EXIT_CODES initLabel(text_line_t *line, labels_t *labels);

EXIT_CODES expandLabelsArray(labels_t *labels);



#endif  // LABELS_H
