#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include <unistd.h>
#include "rgrep.h"
#define DATA_PATH "./data/youtube_"
#define DATA_NUMS 1
#define BUFFER_SIZE 16 * 1024 * 1024

int main(int argc, char **argv) {
    int i = 0, file, offset, buffer = BUFFER_SIZE + 5;

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

    for (i = 0; i < DATA_NUMS; ++i) {
        if (0 == args->l[1]) {
            break;
        } else if (-1 == (file = open(files[i], O_RDONLY)) || NULL == (source = fopen(files[i], "r"))) {
            perror("The following error occurred");
        }

        offset = 0;

        while (true) {
            if (QUERY_SINGLE == args->q.type) {
                if (buffer >= BUFFER_SIZE) {
                    if (-1 == (offset = boost(file, args->q.single, offset))) {
                        break;
                    }

                    fseek(source, offset - ftell(source), SEEK_CUR);

                    offset += BUFFER_SIZE;

                    buffer = 0;
                }
            }

            record = get_record(source, &buffer);

            if (0 == strncmp("eof", record.uid, 3)) {
                break;
            } else if (false == record.error) {
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

        fclose(source);

        close(file);
    }

    fclose(result);

    free_args(&(args->q));

    free(args);

    return 0;
}

int boost(int file, const char *query, int offset) {
    int bytes;

    static char buffer[BUFFER_SIZE + 1] = {'\0'};

    while (true) {
        bytes = read(file, buffer, BUFFER_SIZE);

        if (-1 == bytes) {
            perror("The following error occurred");
        } else if (0 == bytes) {
            break;
        }

        if (NULL != strstr(buffer, query)) {
            break;
        }

        offset += bytes;
    }

    if (0 == bytes) {
        return -1;
    }

    return offset;
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

