#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "rgrep.h"
#define DATA_PATH "/home/freedom/Downloads/data/youtube_"

int main(int argc, char **argv) {
    int i = 0;
    
    double score = 0;

    const char files[][128] = {
        DATA_PATH "1", DATA_PATH "2", DATA_PATH "3", DATA_PATH "4", DATA_PATH "5", DATA_PATH "6", 
        DATA_PATH "7", DATA_PATH "8", DATA_PATH "9", DATA_PATH "10", DATA_PATH "11"
    };

    FILE *source = NULL, *result = NULL;

    struct args *args = parse_args(argc, argv);

    struct youtube record;

    result = fopen("result", "w+");

    if (args->i) {
        query_string_tolower(&(args->q));
    }

    for (i = 0; i < 11; ++i) {
        if (0 == args->l[1]) {
            break;
        } else if (NULL == (source = fopen(files[i], "r"))) {
            perror ("The following error occurred");
        }

        while (true) {
            record = get_record(source);

            if (false == record.error) {
                if (search(record, args, &score)) {
                    if (args->l[0] > 0) {
                        --(args->l[0]);

                        continue;
                    }

                    fprintf(
                            result, 
                            "%s\t%s\t%s\t%s\t%d\t%d\t%d\t%s\t%f\n", 
                            record.id, record.title, record.published, record.content, record.duration, record.favorite, record.view, record.author, score
                           );

                    --(args->l[1]);

                    if (0 == args->l[1]) {
                        break;
                    }
                }
            }
        }

        if (NULL != source) {
            fclose(source);
        }
    }

    fclose(result);

    free_args(&(args->q));

    free(args);

    return 0;
}

void query_string_tolower(struct _query *q) {
    unsigned int i;

    if (NULL != q->single) {
        _tolower(q->single);
    } else {
        for (i = 0; i < q->size; ++i) {
            _tolower(q->multiple[i]->pattern);
        }
    }
}

bool search(struct youtube record, struct args *args, double *score) {
    *score = 0;

    if (args->i) {
        _tolower(record.title);
        _tolower(record.content);
        _tolower(record.author);
    }

    if (QUERY_SINGLE == args->q.type) {
        return _search_single(record, &(args->q));
    } else if (QUERY_BOOLEAN == args->q.type) {
        return _search_boolean(record, &(args->q));
    }

    return _search_multiple(record, &(args->q), score);
}

bool _search_single(struct youtube record, struct _query *q) {
    if (NULL != strstr(record.title, q->single) || NULL != strstr(record.content, q->single)) {
        return true;
    }

    return false;
}

bool _search_boolean(struct youtube record, struct _query *q) {
    unsigned int i;

    char line[MAX_LINE * 2];

    bool result = false;

    strcpy(line, record.title);
    strcat(line, record.content);

    for (i = 0; i < q->size; ++i) {
        result = (QUERY_AND == q->multiple[i]->type)
            ? result && (NULL != strstr(line, q->multiple[i]->pattern))
            : result || (NULL != strstr(line, q->multiple[i]->pattern));
    }

    return result;
}

bool _search_multiple(struct youtube record, struct _query *q, double *score) {
    unsigned int i, len = 0;

    char line[MAX_LINE * 2];

    bool result = false;

    strcpy(line, record.title);
    strcat(line, record.content);

    for (i = 0; i < q->size; ++i) {
        len = strlen(q->multiple[i]->pattern);

        if (QUERY_HAVE == q->multiple[i]->type) {
            if (NULL == strstr(line, q->multiple[i]->pattern)) {
                return false;
            }

            if (NULL != strstr(record.title, q->multiple[i]->pattern)) {
                *score += (double) strlen(record.title) / len * 10;
            }
            
            if (NULL != strstr(record.content, q->multiple[i]->pattern)) {
                *score += (double) strlen(record.content) / len;
            }

            result = true;
        } else if (QUERY_NOT_HAVE == q->multiple[i]->type) {
            if (NULL != strstr(line, q->multiple[i]->pattern)) {
                return false;
            }
        } else if (NULL != strstr(record.title, q->multiple[i]->pattern)) {
            *score += (double) strlen(record.title) / len * 10;

            result = true;
        } else if (NULL != strstr(record.content, q->multiple[i]->pattern)) {
            *score += (double) strlen(record.content) / len;

            result = true;
        }
    }

    return result;
}

void _tolower(char *s) {
    while ('\0' != *s) {
        *s = tolower(*s);

        ++s;
    }
}

