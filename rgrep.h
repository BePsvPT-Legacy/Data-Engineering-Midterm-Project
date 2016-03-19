#ifndef RGREP
#define RGREP 1

#include <stdio.h>
#include "define.h"
#include "lib/core.h"
#include "lib/args.h"
#include "lib/youtube.h"

int main(int, char **);

void query_string_tolower(struct _query *);

bool search(struct youtube, struct args *, double *);

bool _search_single(struct youtube, struct _query *);

bool _search_boolean(struct youtube, struct _query *);

bool _search_multiple(struct youtube, struct _query *, double *);

void _tolower(char *);

#endif

