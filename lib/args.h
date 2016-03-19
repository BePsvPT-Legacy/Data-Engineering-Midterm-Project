#ifndef RGREP_ARGS
#define RGREP_ARGS 1

#include <stdbool.h>

struct _multiple {
    char *pattern;

    /* query string type: and, or. must have, must not have. */
    short int type;
};

struct _query {
    /* determine the query type */
    unsigned short int type;

    /* single keyword */
    char *single;

    /* boolean and multitern query size */
    unsigned int size;

    struct _multiple **multiple;
};

struct args {
    /* begin pattern */
    unsigned long int b;

    /*
     * query string
     * - single keywork
     * - boolean operator
     * - multitern best match
     */
    struct _query q;

    /* insensitive */
    bool i;

    /* ouput from num to num */
    unsigned int l[2];

    /* sorting by */
    unsigned short int r;

    /* output fields */
    bool o[5];

    /* max output size */
    unsigned int s;
};

/* Allocate multiple dimension structures */
struct args *parse_args(const int, char **);

/* Free malloc. */
void free_args(struct args *);

/* Allocate multiple dimension structures for query string. */
void allocate_query_buffer(struct _query *, unsigned int);

/* Get the sepecific character numbers from a string. */
unsigned int get_character_num(const char, const char *);

/* Get the number according to the field. */
int get_field_num(const char *);

#endif

