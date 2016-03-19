#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "rgrep.h"
#define DATA_PATH "/home/freedom/Downloads/data/youtube_"

int main(int argc, char **argv) {
    int i = 0;

    const char files[][128] = {
        DATA_PATH "1", DATA_PATH "2", DATA_PATH "3", DATA_PATH "4", DATA_PATH "5", DATA_PATH "6", 
        DATA_PATH "7", DATA_PATH "8", DATA_PATH "9", DATA_PATH "10", DATA_PATH "11"
    };

    FILE *source = NULL;

    struct args *args = parse_args(argc, argv);

    struct youtube record;

    for (i = 0; i < 11; ++i) {
        if (NULL == (source = fopen(files[i], "r"))) {
            perror ("The following error occurred");
        }

        while (true) {
            record = get_record(source);

            if (false == record.error) {
                if (search(record, args)) {
                }
            }
        }

        if (NULL != source) {
            fclose(source);
        }
    }

    return 0;
}

bool search(struct youtube record, struct args *args) {
    record = record;
    args = args;

    return true;
}

