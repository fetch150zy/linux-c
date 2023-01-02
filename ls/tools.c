#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <errno.h>
#include <assert.h>

#include "./options/include/ops.h"
#include "macro.h"
#include "tools.h"

static bool **color_file(char **file_list, const char *full_path, byte cnt);

// 处理命令行参数
void anlz_args(int argc, char **argv,
                         char **path,
                         char **ops,
                         byte *pcnt, byte *ocnt,
                         bool *has_ops)
{
    // 有传参或者有传入操作符
    if (argc > 1) {
        // 解析参数和操作符（参数和操作符位置无硬性要求）
        for (int i = 1; i < argc; ++i) {
            if ('-' != argv[i][0]) {
                path[(*pcnt)] = (char *)malloc(sizeof(char) * path_max_len);
                strcpy(path[(*pcnt)++], argv[i]);
            } else {
                ops[(*ocnt)] = (char *)malloc(sizeof(char) * op_max_len);
                strcpy(ops[(*ocnt)++], argv[i]);
            }
        }
    }
    // 不传入参数时默认当前目录
    if (1 == argc || 0 == (*pcnt)) {
        path[(*pcnt)] = (char *)malloc(sizeof(char) * 2);
        strcpy(path[(*pcnt)++], ".");
    }

    // (0:-a) (1:-A) (2:-l) (3:-f) (4:-t) (5:-h) (6:invalid)
    bool fg = true;
    for (int i = 0; i < (*ocnt); ++i) {
        if (0 == strcmp(ops[i], "-a") ||
                0 == strcmp(ops[i], "--all"))
            has_ops[0] = true, fg = false;
        if (0 == strcmp(ops[i], "-A") ||
                0 == strcmp(ops[i], "--almost-all"))
            has_ops[1] = true, fg = false;
        if (0 == strcmp(ops[i], "-l") ||
                0 == strcmp(ops[i], "--long"))
            has_ops[2] = true, fg = false;
        if (0 == strcmp(ops[i], "-f") ||
                0 == strcmp(ops[i], "--format"))
            has_ops[3] = true, fg = false;
        if (0 == strcmp(ops[i], "-t") ||
                0 == strcmp(ops[i], "--mtsort"))
            has_ops[4] = true, fg = false;
        if (0 == strcmp(ops[i], "-h") ||
                0 == strcmp(ops[i], "--help"))
            has_ops[5] = true, fg = false;
        if (fg)
            has_ops[6] = true, fg = true;
    }
}


void display_ml(char **path, byte pcnt,
                             char **ops, 
                             byte ocnt, 
                             bool *has_ops)
{
    for (int i = 0; i < pcnt; ++i) {
        char *fullpath = realpath(path[i], NULL);
        if (pcnt > 1) {
            printf("\033[38;5;11m");
            fullpath = realpath(path[i], NULL);
            printf("%s\n", fullpath);
            printf("\033[38;5;7m");
        }
        byte cnt = 0;

        // invalid
        if (has_ops[6]) {
            printf("\033[38;5;9m");
            printf("unknow options\n");
            usage(1);
        }

        // -h
        if (has_ops[5]) usage(0);

        const byte file_list_size = 255;
        const byte file_name_length = 100;
        char **file_list = (char **)malloc(sizeof(char *) * file_list_size);

        // -A（优先级高于-a）（-a高于默认）
        void (*func)(const char *path, byte *cnt,
                                    char **file_list,
                                    const byte file_name_length);
        if (has_ops[1]) func = almost_all;
        else if (has_ops[0]) func = all;
        else func = normal;

        (*func)(path[i], &cnt, file_list, file_name_length);

        // -f
        bool **color_files = color_file(file_list, path[i], cnt);
        char **fmt_files = (char **)malloc(sizeof(char *) * cnt);
        for (byte j = 0; j < cnt; ++j)
            fmt_files[j] = (char *)malloc(sizeof(char) * 1);

        if (has_ops[3])
            format(file_list, path[i], fmt_files, cnt);

        // -l
        if (has_ops[2]) {
            for (byte j = 0; j < cnt; ++j) {
                char tmp[100];
                strcpy(tmp, fullpath);
                strcat(tmp, "/");
                strcat(tmp, file_list[j]);
                long_list(tmp, color_files[j], fmt_files[j]);
            }
        } else {
            for (int j = 0; j < cnt; ++j) {
                if ((j + 1) % 7 == 0)
                    putchar('\n');
                // colorful file
                if (color_files[j][0]) // 文件夹
                    printf("\033[38;5;21m");
                else if (color_files[j][1]) // 管道
                    printf("\033[38;5;166m");
                else if (color_files[j][2]) // 软链接
                    printf("\033[38;5;14m");
                else if (color_files[j][3]) // 可执行文件
                    printf("\033[38;5;118m");
                else
                    printf("\033[38;5;56m");
                strcat(file_list[j], fmt_files[j]);
                printf(" %s ", file_list[j]);
            }
            putchar('\n');
        }

        // 释放内存
        for (int j = 0; j < cnt; ++j)
            free(color_files[j]);
        free(color_files), color_files = NULL;
        for (int j = 0; j < cnt; ++j)
            free(fmt_files[j]);
        free(fmt_files), fmt_files = NULL;
        for (int j = 0; j < cnt; ++j)
            free(file_list[j]);
        free(file_list), file_list = NULL;
        free(fullpath);
    }
}


static bool **color_file(char **file_list, const char *full_path, byte cnt)
{
    bool **color_files = (bool **)malloc(sizeof(bool *) * cnt);
    char *realp = realpath(full_path, NULL);
    for (byte i = 0; i < cnt; ++i) {
        char tmp[100];
        strcpy(tmp, realp);
        strcat(tmp, "/");
        strcat(tmp, file_list[i]);

        char type = get_type(tmp);
        color_files[i] = (bool *)malloc(sizeof(bool) * 4);
        memset(color_files[i], false, sizeof(color_files[i]));

        switch (type)
        {
        case 'd':
            color_files[i][0] = true;
            break;
        case 'p':
            color_files[i][1] = true;
            break;
        case 'l':
            color_files[i][2] = true;
            break;
        default:
            break;
        }

        if ('d' != get_type(tmp) && 
                ('x' == get_mod(tmp)[2] || 
                 'x' == get_mod(tmp)[5] ||
                 'x' == get_mod(tmp)[8]))
            color_files[i][3] = true;
    }

    return color_files;
}
