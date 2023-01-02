#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <dirent.h>

#include <errno.h>
#include <assert.h>

#include "all.h"


static inline DIR *open_dir(const char *path);
static inline void close_dir(DIR *dir);

// 默认
void normal(const char *path, byte *cnt,
                              char **file_list,
                              const byte file_name_length)
{
    DIR *dir = open_dir(path);

    byte i = 0;
    struct dirent *d = NULL;
    while ((d = readdir(dir)) != NULL) {
        if ('.' != d->d_name[0]) {
            file_list[i] = (char *)malloc(sizeof(char) * file_name_length);
            strcpy(file_list[i++], d->d_name);
            ++(*cnt);
        }
    }

    close_dir(dir);
}

// -a
void all(const char *path, byte *cnt,
                           char **file_list,
                           const byte file_name_length)
{
    DIR *dir = open_dir(path);

    byte i = 0;
    struct dirent *d = NULL;
    while ((d = readdir(dir)) != NULL) {
        file_list[i] = (char *)malloc(sizeof(char) * file_name_length);
        strcpy(file_list[i++], d->d_name);
        ++(*cnt);
    }

    close_dir(dir);
}

// -A
void almost_all(const char *path, byte *cnt,
                           char **file_list,
                           const byte file_name_length)
{
    DIR *dir = open_dir(path);

    byte i = 0;
    struct dirent *d = NULL;
    while ((d = readdir(dir)) != NULL) {
        if (0 != strcmp(".", d->d_name) &&
                0 != strcmp("..", d->d_name)) {
            file_list[i] = (char *)malloc(sizeof(char) * file_name_length);
            strcpy(file_list[i++], d->d_name);
            ++(*cnt);
        }
    }

    close_dir(dir);
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