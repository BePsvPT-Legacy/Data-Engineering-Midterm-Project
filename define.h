#ifndef CONSTANT_DEFINED

/* make sure it will not multiple include. */
#define CONSTANT_DEFINED 1

#define MAX_LINE 1024

/* youtube data fields number. */
#define FIELD_SIZE 5

/* transform youtube field to number. */
#define YID 0
#define TITLE 1
#define DESCRIPTION 2
#define KEYWORD 3
#define COUNT 4

/* query type */
#define QUERY_SINGLE 0
#define QUERY_BOOLEAN 1
#define QUERY_MULTIPLE 2
#define QUERY_AND 3
#define QUERY_OR 4
#define QUERY_NORMAL 5
#define QUERY_HAVE 6
#define QUERY_NOT_HAVE 7

#endif

