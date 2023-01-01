#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>

#include <string.h>
#include <errno.h>
#include <assert.h>

#include <stdio.h>
#include <stdlib.h>

extern int errno;

typedef unsigned char byte;

char **dir(const char *path, const char *args, byte *cnt);

int main(int argc, char **argv)
{
    if (argc < 2) {
        perror("usage: ./dir path");
        return -1;
    }

    const char *path = argv[1];
    char **file_list = NULL;
    byte cnt = 0;
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

char **dir(const char *path, const char *args, byte *cnt)
{
    DIR *dir = opendir(path);
    if (NULL == dir) {
        fprintf(stderr, "cause by opendir(): %s\n",
                    strerror(errno));
        assert(0);
    }

    const byte file_list_size = 255;
    const byte file_name_length = 20;
    char **file_list = (char **)malloc(sizeof(char *) * file_list_size);

    byte i = 0;
    if (NULL != args && strcmp("-a", args) == 0) {
        file_list[i] = (char *)malloc(sizeof(char) * file_name_length);
        strcpy(file_list[i++], ".");
        file_list[i] = (char *)malloc(sizeof(char) * file_name_length);
        strcpy(file_list[i++], "..");
        (*cnt) += 2;
    }

    struct dirent *d = NULL;
    while ((d = readdir(dir)) != NULL) {
        if ('.' != d->d_name[0]) {
            file_list[i] = (char *)malloc(sizeof(char) * file_name_length);
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

