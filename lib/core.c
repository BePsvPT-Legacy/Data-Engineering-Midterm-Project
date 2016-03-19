#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "core.h"

char *_strdup(const char *s) {
    size_t len = strlen(s) + 1;

    char *p = malloc(len);

    return p ? memcpy(p, s, len) : NULL;
}

void _free(void *p) {
    if (NULL != p) {
        free(p);
    }
}

