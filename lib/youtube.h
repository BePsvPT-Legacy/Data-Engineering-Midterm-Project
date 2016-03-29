#ifndef RGREP_YOUTUBE
#define RGREP_YOUTUBE 1

#include <stdio.h>
#include <stdbool.h>
#include "../define.h"

struct youtube {
    /* youtube primary key */
    char id[16];

    /* video title */
    char title[256];

    /* published at */
    char published[32];

    /* video description */
    char content[MAX_LINE];

    /* video length (seconds) */
    unsigned int duration;

    /* favorite count */
    unsigned int favorite;

    /* view count */
    unsigned int view;

    /* uploader */
    char author[64];

    char uid[16];

    bool error;
};

struct youtube get_record(FILE *, int *);

#endif

