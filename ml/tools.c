#include <sys/types.h>
#include <sys/stat.h>
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

static bool **color_file(const char * const *file_list, const char *real_path,
                                                        int cnt);

static bool* color_single_file(const char *full_path);

// 处理命令行参数
bool anlz_args(int argc, char **argv, char **paths, int *pcnt, bool *has_ops,
                                                               char *single_file)
{
    bool is_single = false;
    char **ops = (char **)malloc(sizeof(char *) * OP_NUM);
    int ocnt = 0;
    // 有传参或者有传入操作符
    if (argc > 1) {
        // 解析参数和操作符（参数和操作符位置无硬性要求）
        for (int i = 1; i < argc; ++i) {
            if ('-' != argv[i][0]) { // 参数
                if (0 == access(argv[i], F_OK)) { // 文件或目录存在
                    char real_path[PATH_MAX_LEN];
                    realpath(argv[i], real_path);
                    struct stat myst; 
                    stat(real_path, &myst); 
                    if(S_IFDIR & myst.st_mode){ // 是文件夹
                        paths[(*pcnt)] = (char *)malloc(sizeof(char) * PATH_MAX_LEN);
                        strcpy(paths[(*pcnt)++], argv[i]);
                    } else { // 是文件
                        strcpy(single_file, argv[1]);
                        is_single = true;
                    }    
                } else { // 文件或目录不存在
                    has_ops[7] = true;
                }
            } else { // 操作符
                ops[ocnt] = (char *)malloc(sizeof(char) * OP_MAX_LEN);
                strcpy(ops[ocnt++], argv[i]);
            }
        }
    }
    // 不传入参数时默认当前目录
    if (1 == argc || 0 == (*pcnt)) {
        paths[(*pcnt)] = (char *)malloc(sizeof(char) * 2);
        strcpy(paths[(*pcnt)++], ".");
    }

    // (0:-a) (1:-A) (2:-l) (3:-f) (4:-t) (5:-h) (6:op invalid)
    bool fg = true;
    for (int i = 0; i < ocnt; ++i) {
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
                0 == strcmp(ops[i], "--ctsort"))
            has_ops[4] = true, fg = false;
        if (0 == strcmp(ops[i], "-h") ||
                0 == strcmp(ops[i], "--help"))
            has_ops[5] = true, fg = false;
        if (fg)
            has_ops[6] = true;
        fg = true;
    }

    for (int i = 0; i < ocnt; ++i)
        free(ops[i]);
    free(ops), ops = NULL;

    return is_single;
}


void allpath_display(const char * const *paths, int pcnt, const bool *has_ops)
{
    // 遍历所有传入的路径
    for (int i = 0; i < pcnt; ++i) {
        // 把当前路径转换为绝对路径
        char *real_path = realpath(paths[i], NULL);
        // 如果是多路径，每次输出文件列表前先输出当前所在绝对路径
        if (pcnt > 1 && !has_ops[7]) {
            printf("\033[38;5;11m");
            printf("%s\n", real_path);
        }
        strcat(real_path, "/");

        // 文件或目录不存在
        if (has_ops[7]) {
            printf("ml error: No such file or directory.\n");
            exit(1);
        }

        // 输入操作符非法
        if (has_ops[6]) {
            printf("\033[38;5;9m");
            printf("unknow options\n");
            // 顺带打印使用指南
            usage(1);
        }

        // -h (--help)，使用指南
        if (has_ops[5])
            usage(0);

        // 当前路径下文件或文件夹个数
        int cnt = 0;
        // 文件或文件夹列表
        char **file_list = (char **)malloc(sizeof(char *) * FILE_LIST_SIZE);

        // -A（优先级高于-a）（-a高于默认）
        // 文件输出布局（主要用于 %-*s 格式化输出文件或目录）
        int layout[LINE_MAXN];
        void (*func)(const char *path, int *cnt, char **file_list);
        if (has_ops[1])
            func = almost_all;
        else if (has_ops[0])
            func = all;
        else
            func = normal;
        (*func)(paths[i], &cnt, file_list);

        // -f (--format)，格式化输出
        char **fmt_files = (char **)malloc(sizeof(char *) * cnt);
        for (int j = 0; j < cnt; ++j) {
            fmt_files[j] = (char *)malloc(sizeof(char) * 2);
            fmt_files[j][0] = fmt_files[j][1] = '\0';
        }
        if (has_ops[3])
            format((const char * const *)file_list, real_path, fmt_files, cnt);

        // -t (--ctsort)，按照文件或文件及最后修改时间升序排列
        if (has_ops[4])
            ctime_sort(cnt, real_path, file_list);

        // 规范输出
        // 一行输出最多文件或文件夹数
        int sum_length = 0, line_maxn;
        int mtx[FILE_LIST_SIZE];

        for (int j = 0; j < cnt; ++j) {
            mtx[j] = strlen(file_list[j]);
            sum_length += mtx[j];
        }
        int rows = (sum_length / LINE_MAX_LENGTH) + 1;
        if (0 == cnt % rows)
            line_maxn = cnt / rows;
        else 
            line_maxn = cnt / rows + 1;

        for (int i = 0; i < line_maxn; ++i) {
            layout[i] = 0;
            for (int j = 0; j < rows; ++j) {
                if (layout[i] < mtx[j * line_maxn + i] &&
                        (j * line_maxn + i) < cnt)
                    layout[i] = mtx[j * line_maxn + i];
            }
        }


        // 特殊文件彩色显示
        bool **color_files = color_file((const char * const *)file_list, real_path, cnt);

        // -l
        if (has_ops[2]) {
            int fmt_lens[] = {0, 0, 0};
            get_fmt_lens(real_path, (const char * const *)file_list, cnt, fmt_lens); 
            for (int j = 0; j < cnt; ++j) {
                char tmp[PATH_MAX_LEN + FILE_NAME_LEN];
                strcpy(tmp, real_path);
                strcat(tmp, file_list[j]);
                long_list(tmp, color_files[j], fmt_files[j], fmt_lens);
            }
        } else {
            for (int j = 0; j < cnt; ++j) {
                if (0 == j % line_maxn && 0 != j)
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

                printf("%-*s", layout[j % line_maxn] + 4, file_list[j]);
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

        free(real_path);
    }
}


void single_file_display(const char *single_file, const bool *has_ops)
{
    char *real_path = realpath(".", NULL);
    strcat(real_path, "/");
    char full_path[PATH_MAX_LEN + FILE_NAME_LEN];
    strcpy(full_path, real_path);
    strcat(full_path, single_file);

    // 文件或目录不存在
    if (has_ops[7]) {
        printf("ml error: No such file or directory.\n");
        exit(1);
    }

    // 输入操作符非法
    if (has_ops[6]) {
        printf("\033[38;5;9m");
        printf("unknow options\n");
        // 顺带打印使用指南
        usage(1);
    }

    // -h (--help)，使用指南
    if (has_ops[5])
        usage(0);

    // -f (--format)，格式化输出
    char fmt_file[2];
    if (has_ops[3])
        format_single_file(full_path, fmt_file);

    // 文件彩色输出
    bool *color_files = color_single_file(full_path);

    // -l
    if (has_ops[2]) {
        int fmt_lens[] = {0, 0, 0};
        get_fmt_lens(real_path, (const char * const *)&single_file, 1, fmt_lens);
        long_list(full_path, color_files, fmt_file, fmt_lens);
    } else {
        if (color_files[0]) // 文件夹
            printf("\033[38;5;21m");
        else if (color_files[1]) // 管道
            printf("\033[38;5;166m");
        else if (color_files[2]) // 软链接
            printf("\033[38;5;14m");
        else if (color_files[3]) // 可执行文件
            printf("\033[38;5;118m");
        else
            printf("\033[38;5;56m");

        char tmp[FILE_NAME_LEN];
        strcpy(tmp, single_file);
        strcat(tmp, fmt_file);

        printf("%-*s\n", sizeof(tmp) + 3, tmp);
    }

    free(color_files);
    free(real_path);
}


static bool **color_file(const char * const *file_list, const char *real_path,
                                                        int cnt)
{
    bool **color_files = (bool **)malloc(sizeof(bool *) * cnt);
    for (int i = 0; i < cnt; ++i) {
        char tmp[PATH_MAX_LEN + FILE_NAME_LEN];
        strcpy(tmp, real_path);
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
            if ('x' == get_mod(tmp)[2] || 'x' == get_mod(tmp)[5] ||
                                          'x' == get_mod(tmp)[8])
                color_files[i][3] = true;
            break;
        }
    }

    return color_files;
}


static bool *color_single_file(const char *full_path)
{
    bool *color_files = (bool *)malloc(sizeof(bool) * 4);
    char type = get_type(full_path);

    switch (type)
    {
    // 软链接
    case 'l':
        color_files[0] = true;
        break;
    // 文件夹
    case 'd':
        color_files[1] = true;
        break;
    // 管道
    case 'p':
        color_files[2] = true;
        break;
    default:
        if ('x' == get_mod(full_path)[2] || 'x' == get_mod(full_path)[5] ||
                                        'x' == get_mod(full_path)[8])
            color_files[3] = true;
        break;
    }

    return color_files;
}
