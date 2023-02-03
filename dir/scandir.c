#include <dirent.h>
#include <string.h>
#include <unistd.h>

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "../utils/utils.h"

//int scandir(const char *drip, struct dirent ***namelist,
//        int (*filter)(const struct dirent *),
//        int (*compar)(const struct dirent **, const struct dirent **));


static const int usage_failed = 1;
static const int scandir_failed = 2;

static int is_png(const struct dirent *dp);
static bool scan_dir(const char *dir_name);


int main(int argc, char **argv)
{
    exit_if(argc < 2, usage_failed, stderr, "usage: %s <path>", argv[0])

    bool ret = scan_dir(argv[1]);
    if (!ret)
        return scandir_failed;

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