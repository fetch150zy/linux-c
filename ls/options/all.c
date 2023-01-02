#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <dirent.h>

#include <errno.h>
#include <assert.h>

#include "all.h"
#include "../macro.h"


static inline DIR *open_dir(const char *path);
static inline void close_dir(DIR *dir);

// 默认
int normal(const char *path, int *cnt,
                             char **file_list,
                             int *layout)
{
    int sum_length = 0, line_maxn;
    int mtx[256];
    DIR *dir = open_dir(path);

    int i = 0;
    struct dirent *d = NULL;
    while ((d = readdir(dir)) != NULL) {
        if ('.' != d->d_name[0]) {
            file_list[i] = (char *)malloc(sizeof(char) * FILE_NAME_LENGTH);
            strcpy(file_list[i++], d->d_name);
            mtx[(*cnt)] = (int)(strlen(d->d_name));
            sum_length += mtx[(*cnt)++];
        }
    }
    int rows = (sum_length / LINE_MAX_LENGTH) + 1;
    if (0 == (*cnt) % rows)
        line_maxn = (*cnt) / rows;
    else 
        line_maxn = (*cnt) / rows + 1;

    for (int i = 0; i < line_maxn; ++i) {
        layout[i] = 0;
        for (int j = 0; j < rows; ++j) {
            if (layout[i] < mtx[j * line_maxn + i] &&
                    (j * line_maxn + i) < (*cnt))
                layout[i] = mtx[j * line_maxn + i];
        }
    }

    close_dir(dir);

    return line_maxn;
}

// -a
int all(const char *path, int *cnt,
                          char **file_list,
                          int *layout)
{
    int sum_length = 0, line_maxn;
    int mtx[255];
    DIR *dir = open_dir(path);

    int i = 0;
    struct dirent *d = NULL;
    while ((d = readdir(dir)) != NULL) {
        file_list[i] = (char *)malloc(sizeof(char) * FILE_NAME_LENGTH);
        strcpy(file_list[i++], d->d_name);
        mtx[(*cnt)] = strlen(d->d_name);
        sum_length += mtx[(*cnt)++];
    }
    int rows = (sum_length / LINE_MAX_LENGTH) + 1;
    if (0 == (*cnt) % rows)
        line_maxn = (*cnt) / rows;
    else 
        line_maxn = (*cnt) / rows + 1;

    for (int i = 0; i < line_maxn; ++i) {
        layout[i] = 0;
        for (int j = 0; j < rows; ++j) {
            if (layout[i] < mtx[j * line_maxn + i] &&
                    (j * line_maxn + i) < (*cnt))
                layout[i] = mtx[j * line_maxn + i];
        }
    }

    close_dir(dir);

    return line_maxn;
}

// -A
int almost_all(const char *path, int *cnt,
                                 char **file_list,
                                 int *layout)
{
    int sum_length = 0, line_maxn;
    int mtx[255];
    DIR *dir = open_dir(path);

    int i = 0;
    struct dirent *d = NULL;
    while ((d = readdir(dir)) != NULL) {
        if (0 != strcmp(".", d->d_name) &&
                0 != strcmp("..", d->d_name)) {
            file_list[i] = (char *)malloc(sizeof(char) * FILE_NAME_LENGTH);
            strcpy(file_list[i++], d->d_name);
            mtx[(*cnt)] = strlen(d->d_name);
            sum_length += mtx[(*cnt)++];
        }
    }
    int rows = (sum_length / LINE_MAX_LENGTH) + 1;
    if (0 == (*cnt) % rows)
        line_maxn = (*cnt) / rows;
    else 
        line_maxn = (*cnt) / rows + 1;

    for (int i = 0; i < line_maxn; ++i) {
        layout[i] = 0;
        for (int j = 0; j < rows; ++j) {
            if (layout[i] < mtx[j * line_maxn + i] &&
                    (j * line_maxn + i) < (*cnt))
                layout[i] = mtx[j * line_maxn + i];
        }
    }

    close_dir(dir);

    return line_maxn;
}


static inline DIR *open_dir(const char *path)
{
    // opendir
    DIR *dir = opendir(path);
    if (NULL == dir) {
        fprintf(stderr, "cause by opendir(): %s\n",
                    strerror(errno));
        assert(0);
    }

    return dir;
}


static inline void close_dir(DIR *dir)
{
    // closedir
    int err_ = closedir(dir);
    if (-1 == err_) {
        fprintf(stderr, "cause by closedir(): %s\n",
                    strerror(errno));
        assert(0);
    }
}