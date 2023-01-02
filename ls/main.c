#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#include "tools.h"

#include "macro.h"


int main(int argc, char **argv)
{
    // 存 path 和 options
    char **path = (char **)malloc(sizeof(char *) * path_num);
    char **ops = (char **)malloc(sizeof(char *) * op_num);
    // path 和 options 的数目
    byte pcnt = 0, ocnt = 0;
    // (0:-a) (1:-A) (2:-l) (3:-f) (4:-t) (5:-h) (6:invalid)
    bool has_ops[7] = {false, false, false,
                              false, false, false, false};

    anlz_args(argc, argv, path, ops, &pcnt, &ocnt, has_ops);

    display_ml(path, pcnt, ops, ocnt, has_ops);

    // 释放内存
    for (int i = 0; i < pcnt; ++i)
        free(path[i]);
    free(path), path = NULL;

    for (int i = 0; i < ocnt; ++i)
        free(ops[i]);
    free(ops), ops = NULL;

    return 0;
}

