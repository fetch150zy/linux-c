#include <stdlib.h>
#include <sys/stat.h>
#include <stdio.h>
#include <time.h>
#include <string.h>

#include "../macro.h"

static inline int cmpfunc(const void * file_name1, const void * file_name2);
static inline time_t get_ctime_num(const char *file_name);

// 排序需要（目前没有想到比全局变量更好的方式）
// 为了防止外界引用，声明为static，大小为最大路径长度加上最大文件长度
static char fp[PATH_MAX_LEN + FILE_NAME_LEN];

// 根据文件或文件夹最近修改时间升序排列
void ctime_sort(int cnt, const char * real_path, char **file_list)
{
    // 把当前操作目录拷贝到fp中
    strcpy(fp, real_path);

    qsort(file_list, cnt, sizeof(char *), cmpfunc);
}

// 比较函数
static inline int cmpfunc(const void * file_name1, const void * file_name2)
{
    const char** a = (const char**) file_name1;
    const char** b = (const char**) file_name2;
    return ( get_ctime_num(*a) - get_ctime_num(*b) );
}

// 获取文件（目录）最近修改时间
static inline time_t get_ctime_num(const char *file_name)
{
    char tmp[PATH_MAX_LEN + FILE_NAME_LEN];
    strcpy(tmp, fp);
    // printf("%s\n", tmp);
    strcat(tmp, file_name);
    // printf("%s\n", tmp);
    struct stat myst;
    int ret = stat(tmp, &myst);
    if (-1 == ret) {
        perror("stat(): ctime_sort.c get_ctime_num()");
        return 0l;
    }
    return myst.st_ctime;
}
