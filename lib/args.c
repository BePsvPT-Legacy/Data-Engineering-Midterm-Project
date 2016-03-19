#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../define.h"
#include "core.h"
#include "args.h"

struct args *parse_args(const int argc, char **argv) {
    int c = 0, i = 0, temp_i = 0;

    char *temp_p = NULL;

    struct args *args = (struct args *)malloc(sizeof(struct args));

    args->q.single = NULL;
    args->q.size = 0;
    args->q.multiple = NULL;
    args->i = false;
    args->l[0] = args->l[1] = 0;
    args->r = YID;
    args->o[YID] = args->o[TITLE] = args->o[DESCRIPTION] = args->o[KEYWORD] = args->o[COUNT] = false;
    args->s = 0;

    for (i = 1; i < argc; ++i) {
        if ('-' == argv[i][0]) {
            switch (argv[i][1]) {
                case 'q':
                    ++i; /* right shift argv */

                    /* 
                     * The q parameter has three formats.
                     * Single match:    -q "query_string"
                     * Boolean match:   -q "first|second|third&fourth|fifth"
                     * Multiple match:  -q "first,+second,-third,fourth,+fifth"
                     */
                    if (NULL == strstr(argv[i], ",")) {
                        if (NULL == strstr(argv[i], "|") && NULL == strstr(argv[i], "&")) {
                            args->q.type = QUERY_SINGLE;
                            args->q.single = _strdup(argv[i]);
                        } else {
                            int n = 0, m = 0;
                            
                            short int last_pattern_type = QUERY_AND;

                            char copy[MAX_LINE];

                            args->q.type = QUERY_BOOLEAN;
                            
                            allocate_query_buffer(
                                &(args->q), 
                                get_character_num('|', argv[i]) + get_character_num('&', argv[i]) + 1
                            );

                            /* Seperate string by | and & characters, then store it. */
                            for (c = 0, temp_i = strlen(argv[i]) + 1; c < temp_i && c < MAX_LINE; ++c, ++n) {
                                copy[n] = argv[i][c];

                                if ('|' == argv[i][c] || '&' == argv[i][c] || '\0' == argv[i][c] || c == MAX_LINE - 1) {
                                    copy[n] = '\0';

                                    args->q.multiple[m]->pattern = _strdup(copy);
                                    args->q.multiple[m]->type = last_pattern_type;
                                    last_pattern_type = ('|' == argv[i][c]) ? QUERY_OR : QUERY_AND;

                                    n = -1;

                                    ++m;
                                }
                            }
                        }
                    } else {
                        args->q.type = QUERY_MULTIPLE;

                        allocate_query_buffer(
                            &(args->q),
                            get_character_num(',', argv[i]) + 1
                        );

                        c = 0;

                        /* Seperate string by comma and store it. */
                        temp_p = strtok(argv[i], ",");

                        while (NULL != temp_p) {
                            if ('+' == temp_p[0] || '-' == temp_p[0]) {
                                args->q.multiple[c]->type = ('+' == temp_p[0]) ? QUERY_HAVE : QUERY_NOT_HAVE;

                                ++temp_p;
                            }

                            args->q.multiple[c]->pattern = _strdup(temp_p);

                            ++c;

                            temp_p = strtok(NULL, ",");
                        }
                    }

                    break;

                case 'i':
                    args->i = true;

                    break;

                case 'l':
                    ++i; /* right shift argv */

                    /*
                     * If there is no comma, we assume that it is the output number limit.
                     * When there is comma, the format is "begin number,limit", like 10,50
                     * means begin from 10 and output 50 records.
                     */
                    if (NULL == strstr(argv[i], ",")) {
                        args->l[1] = strtoul(argv[i], NULL, 0);
                    } else {
                        args->l[0] = strtoul(strtok(argv[i], ","), NULL, 0);
                        args->l[1] = strtoul(strtok(NULL, ","), NULL, 0);
                    }

                    break;

                case 'r':
                    ++i; /* right shift argv */

                    /* Sorting by which field, default is yid field. */
                    temp_i = get_field_num(argv[i]);

                    if (-1 != temp_i) {
                        args->r = temp_i;
                    }

                    break;
                
                case 'o':
                    ++i; /* right shift argv */

                    /* The fields that should output, default is none. */
                    temp_p = strtok(argv[i], ",");

                    while (NULL != temp_p) {
                        temp_i = get_field_num(temp_p);

                        if (-1 != temp_i) {
                            args->o[temp_i] = true;
                        }

                        temp_p = strtok(NULL, ",");
                    }

                    break;

                case 's':
                    ++i; /* right shift argv */

                    /* The each record max output bytes. */
                    args->s = strtoul(argv[i], NULL, 0);

                    break;
            }
        }
    }

    return args;
}

/* Free malloc. */
void free_args(struct args *args) {
    unsigned int i;

    for (i = 0; i < args->q.size; ++i) {
        _free(&(args->q.multiple[i]->pattern));
        args->q.multiple[i]->pattern = NULL;

        _free(&(args->q.multiple[i]));
        args->q.multiple[i] = NULL;
    }

    _free(&(args->q.multiple));
    args->q.multiple = NULL;

    _free(&(args->q.single));
    args->q.single = NULL;
}

/* Allocate multiple dimension structures for query string. */
void allocate_query_buffer(struct _query *p, unsigned int size) {
    unsigned int i = 0;

    p->size = size;

    p->multiple = (struct _multiple **)malloc(sizeof(struct _multiple *) * size);

    for (i = 0; i < size; ++i) {
        p->multiple[i] = (struct _multiple *)malloc(sizeof(struct _multiple));

        p->multiple[i]->type = QUERY_NORMAL;
    }
}

/* Get the sepecific character numbers from a string. */
unsigned int get_character_num(const char c, const char *s) {
    int times = 0;

    while ('\0' != *s) {
        if (c == *s) {
            ++times;
        }

        ++s;
    }

    return times;
}

/* Get the number according to the field. */
int get_field_num(const char *field) {
    int c = 0;

    static const char fields[FIELD_SIZE][16] = {"yid", "title", "description", "keyword", "count"};

    for (c = 0; c < FIELD_SIZE; ++c) {
        if (0 == strcmp(fields[c], field)) {
            return c;
        }
    }

    return -1;
}

