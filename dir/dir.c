#include <dirent.h>
#include <string.h>
#include <errno.h>
#include <assert.h>

#include <stdio.h>
#include <stdlib.h>

extern int errno;

char **dir(const char *path, const char *args, int *cnt);

int main(int argc, char **argv)
{
    if (argc < 2) {
        perror("usage: ./dir path");
        return -1;
    }

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
    if (NULL == dir) {
        fprintf(stderr, "cause by opendir(): %s\n",
                    strerror(errno));
        assert(0);
    }

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
    int err_ = closedir(dir);
    if (-1 == err_) {
        fprintf(stderr, "cause by closedir(): %s\n",
                    strerror(errno));
        assert(0);
    }

    return file_list;
}

