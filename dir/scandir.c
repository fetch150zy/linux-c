//
// Created by fetch150zy on 1/8/23.
//

#include <dirent.h>
#include <string.h>
#include <unistd.h>

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

//int scandir(const char *drip, struct dirent ***namelist,
//        int (*filter)(const struct dirent *),
//        int (*compar)(const struct dirent **, const struct dirent **));


int is_png(const struct dirent *dp);

bool scan_dir(const char *dir_name);


int main(int argc, char **argv)
{
    if (argc < 2) {
        printf("usage: ./scandir <path>\n");
        return -1;
    }

    bool ret = scan_dir(argv[1]);
    if (!ret)
        return -1;

    return 0;
}


bool scan_dir(const char *dir_name)
{
    // there are two default compare function
    // alphasort()
    // versionsort()

    // check the path is available
    if (0 != access(dir_name, F_OK)) {
        perror("the path is't available");
        goto err_ret;
    }

    // the scandir function will alloc memory for it, remeber free
    struct dirent ** namelist = NULL;
    int ret = scandir(dir_name, &namelist, is_png, alphasort);
    if (-1 == ret) {
        perror("no match");
        goto err_ret;
    } else {
        for (int i = 1; i <= ret; ++i) {
            printf("%d: %s\n", i, namelist[i - 1]->d_name);
            free(namelist[i - 1]);
        }
        free(namelist);
        goto sucess_ret;
    }

err_ret:
    return false;

sucess_ret:
    return true;
}


int is_png(const struct dirent *dp)
{
    if (DT_REG == dp->d_type) {
        char *p = strstr(dp->d_name, ".png");
        if (NULL != p && '\0' == *(p + 4))
            return 1;
    }
    return 0;
}