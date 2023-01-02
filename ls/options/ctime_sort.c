#include <stdlib.h>
#include <sys/stat.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#include "long.h"
#include "../macro.h"

static inline int cmpfunc(const void * a, const void * b);
static inline time_t get_ctime_num(const char *file);

char fp[PATH_MAX_LEN];

void ctime_sort(int cnt, char *full_path, char **file_list)
{
    strcpy(fp, full_path);

    qsort(file_list, cnt, sizeof(time_t), cmpfunc);
}


static inline time_t get_ctime_num(const char *file)
{
    char tmp[100];
    strcpy(tmp, fp);
    strcat(tmp, file);
    struct stat myst;
    int ret = stat(tmp, &myst);
    return myst.st_ctime;
}


static inline int cmpfunc(const void * a, const void * b)
{
   return ( get_ctime_num((char*)a) - get_ctime_num((char*)b) );
}
