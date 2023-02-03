#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "tools.h"
#include "macro.h"


int main(int argc, char **argv)
{
    // 存 path 和 options
    char **path = (char **)malloc(sizeof(char *) * PATH_NUM);
    char *single_file = (char *)malloc(sizeof(char) * FILE_NAME_LEN);
    // path 和 options 的数目
    int pcnt = 0;
    // (0:-a) (1:-A) (2:-l) (3:-f) (4:-t) (5:-h) (6:op invalid) (7: args invalid)
    bool has_ops[] = {false, false, false,
                              false, false, false, false, false};

    bool is_signle = anlz_args(argc, argv, path, &pcnt, has_ops, single_file);

    if (is_signle) {
        single_file_display(single_file, has_ops);
        // 释放signle_file
        free(single_file), single_file = NULL;
    } else {
        allpath_display((const char * const *)path, pcnt, has_ops);

        // 释放path
        for (int i = 0; i < pcnt; ++i)
            free(path[i]), path[i] = NULL;
        free(path), path = NULL;
    }

    return 0;
}

