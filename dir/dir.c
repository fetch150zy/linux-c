#include <dirent.h>
#include <string.h>
#include <errno.h>
#include <assert.h>

#include <stdio.h>
#include <stdlib.h>

#include "../utils/utils.h"

extern int errno;

static const int usage_err = 1;
static const int opendir_failed = 2;
static const int readdir_failed = 4;
static const int closedir_failed = 8;

char **dir(const char *path, const char *args, int *cnt);

int main(int argc, char **argv)
{
    exit_if(argc < 2, usage_err, stderr, "usage: %s <path>", argv[0])

    const char *path = argv[1];
    char **file_list = NULL;
    int cnt = 0;
    if (argc == 2) {
        file_list = dir(path, NULL, &cnt);
    } else {
        const char *args = argv[2];
        file_list = dir(path, args, &cnt);
    }

    for (int i = 0; i < cnt; ++i)
        printf("%s  ", file_list[i]);
    putchar('\n');

    for (int i = 0; i < cnt; ++i)
        free(file_list[i]);
    free(file_list);
    file_list = NULL;

    return 0;
}

char **dir(const char *path, const char *args, int *cnt)
{
    DIR *dir = opendir(path);
    exit_if(NULL == dir, opendir_failed, stderr, "opendir failed\n")

    const int FILE_LIST_SIZE = 255;
    const int FILE_NAME_LENGTH = 20;
    char **file_list = (char **)malloc(sizeof(char *) * FILE_LIST_SIZE);

    int i = 0;
    if (NULL != args && strcmp("-a", args) == 0) {
        file_list[i] = (char *)malloc(sizeof(char) * FILE_NAME_LENGTH);
        strcpy(file_list[i++], ".");
        file_list[i] = (char *)malloc(sizeof(char) * FILE_NAME_LENGTH);
        strcpy(file_list[i++], "..");
        (*cnt) += 2;
    }

    struct dirent *d = NULL;
    while ((d = readdir(dir)) != NULL) {
        if ('.' != d->d_name[0]) {
            file_list[i] = (char *)malloc(sizeof(char) * FILE_NAME_LENGTH);
            strcpy(file_list[i++], d->d_name);
            ++(*cnt);
        }
    }
    exit_if(-1 == closedir(dir), closedir_failed, stderr, "closedir failed\n")

    return file_list;
}

