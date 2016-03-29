#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../define.h"
#include "youtube.h"

struct youtube get_record(FILE *file, int *buffer) {
    int len = 0;

    char line[MAX_LINE], *temp_p;

    struct youtube record;

    record.error = true;

    if (feof(file)) {
        strcpy(record.uid, "eof");

        return record;
    }

    while (NULL != fgets(line, MAX_LINE, file)) {
        len = strlen(line);

        if ('\0' != line[len-1]) {
            line[len-1] = '\0';
        }

        *buffer += len;

        if (0 == strcmp("@", line)) {
            break;
        }

        if (0 == strncmp(line, "@id:", 4)) {
            temp_p = &line[4];

            strncpy(record.id, temp_p, 16);
        } else if (0 == strncmp(line, "@title:", 7)) {
            temp_p = &line[7];

            strncpy(record.title, temp_p, 256);
        } else if (0 == strncmp(line, "@published:", 11)) {
            temp_p = &line[11];

            strncpy(record.published, temp_p, 32);
        } else if (0 == strncmp(line, "@content:", 9)) {
            temp_p = &line[9];

            strncpy(record.content, temp_p, MAX_LINE);
        } else if (0 == strncmp(line, "@duration:", 10)) {
            temp_p = &line[10];

            record.duration = strtoul(temp_p, NULL, 0);
        } else if (0 == strncmp(line, "@favoriteCount:", 15)) {
            temp_p = &line[15];

            record.favorite = strtoul(temp_p, NULL, 0);
        } else if (0 == strncmp(line, "@$viewCount:", 12)) {
            temp_p = &line[12];

            record.view = strtoul(temp_p, NULL, 0);
        } else if (0 == strncmp(line, "@author:", 8)) {
            temp_p = &line[8];

            strncpy(record.author, temp_p, 64);
        } else if (0 == strncmp(line, "@_uid:", 6)) {
            temp_p = &line[6];

            strncpy(record.uid, temp_p, 16);

            record.error = false;
        }
    }

    return record;
}

