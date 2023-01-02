#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <pwd.h> // struct passwd
#include <grp.h> // struct group
#include <time.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "long.h"
#include "../alias.h"


static char *uid_to_name(uid_t uid);
static char *gid_to_name(gid_t gid);

void long_list(const char *full_path, bool *color_file,
                                      char *fmt_file)
{
    char file_type = get_type(full_path);
    printf("\033[38;5;7m");
    printf("%c", file_type);

    char *file_mod = get_mod(full_path);
    for (int i = 0; i < 3; ++i) {
        printf("\033[38;5;8m");
        if (file_mod[i*3] == 'r')
            printf("\033[38;5;10m");
        printf("%c", file_mod[i*3]);
        printf("\033[38;5;8m");

        if (file_mod[i*3+1] == 'w')
            printf("\033[38;5;11m");
        printf("%c", file_mod[i*3+1]);
        printf("\033[38;5;8m");

        if (file_mod[i*3+2] == 'x')
            printf("\033[38;5;9m");
        printf("%c", file_mod[i*3+2]);
    }
    putchar(' ');

    char *ower = get_ower(full_path);
    printf("\033[38;5;7m");
    printf("%s", ower);

    char *grp = get_grp(full_path);
    printf("\033[38;5;239m");
    printf(" %s ", grp);

    u_int32_t fsize = get_file_size(full_path);
    printf("\033[38;5;228m");
    printf("%.1lf KB", (fsize / 1024.0));

    char *format = get_time(full_path);
    printf("\033[38;5;123m");
    printf(" %s", format);

    // 拿到路径最后文件名
    char split_ch = '/';
    char *file = strrchr(full_path, split_ch) + 1;
    if (color_file[0]) // 文件夹
        printf("\033[38;5;21m");
    else if (color_file[1]) // 管道
        printf("\033[38;5;166m");
    else if (color_file[2]) // 软链接
        printf("\033[38;5;14m");
    else if (color_file[3]) // 可执行文件
        printf("\033[38;5;118m");
    else
        printf("\033[38;5;56m");
    strcat(file, fmt_file);
    printf(" %s\n", file);

    free(format);
    format = NULL;
    free(file_mod);
    file_mod = NULL;
}

// 获取文件种类
char get_type(const char *full_path)
{
    struct stat myst;
    int ret = lstat(full_path, &myst);
    if (-1 == ret) {
        perror("stat");
        return '\0';
    }

    char file_type;

    switch (myst.st_mode & S_IFMT)
    {
    case S_IFBLK:
        file_type = 'b';
        break;
    case S_IFCHR:
        file_type = 'c';
        break;
    case S_IFDIR:
        file_type = 'd';
        break;
    case S_IFIFO:
        file_type = 'p';
        break;
    case S_IFLNK:
        file_type = 'l';
        break;
    case S_IFREG:
        file_type = '.';
        break;
    case S_IFSOCK:
        file_type = 's';
        break;
    default:
        perror("Unknown type");
        assert(0);
        break;
    }

    return file_type;
}

// 获取文件或目录权限
char* get_mod(const char *full_path)
{
    struct stat myst;
    int ret = stat(full_path, &myst);
    if (-1 == ret) {
        perror("stat");
        return NULL;
    }
    const byte mode_size = 10;
    char *file_mod = (char *)malloc(sizeof(char) * mode_size);
    const char mode[] = "rwx";
    int t = 0000400, st_mode = myst.st_mode;
    int idx = 0, move = 8;
    while (t != 0) {
        for (int i = 0; i < 3; ++i) {
            file_mod[idx++] = ((((st_mode & t) >> (move--)) == 1) 
                    ? mode[i] : '-');
            t >>= 1;
        }
    }
    file_mod[idx] = '\0';
    return file_mod;
}

// 获取文件或目录所属用户
char *get_ower(const char *full_path)
{
    struct stat myst;
    int ret = stat(full_path, &myst);
    if (-1 == ret) {
        perror("stat");
        return NULL;
    }

    return uid_to_name(myst.st_uid);
}

// 获取文件或目录所属组
char *get_grp(const char *full_path)
{
    struct stat myst;
    int ret = stat(full_path, &myst);
    if (-1 == ret) {
        perror("stat");
        return NULL;
    }

    return gid_to_name(myst.st_gid);
}

// 获取文件或目录大小
unsigned int get_file_size(const char *full_path)
{
    struct stat myst;
    int ret = stat(full_path, &myst);
    if (-1 == ret) {
        perror("stat");
        return -1;
    }
    return (u_int32_t)myst.st_size;
}

// 获取文件或目录最后修改时间
char *get_time(const char *full_path)
{
    struct stat myst;
    int ret = stat(full_path, &myst);
    if (-1 == ret) {
        perror("stat");
        return NULL;
    }
    const char *wday[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri",
                            "Sat"};
    const char *ymoth[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun",
                            "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
    time_t t = myst.st_ctime;
    struct tm *p = localtime(&t);
    const byte time_fmt_buf_size = 30;
    char *format = (char *)malloc(sizeof(char) * time_fmt_buf_size);
    sprintf(format, "%s %s %#2d %02d:%02d:%02d %d", 
            wday[p->tm_wday], ymoth[(p->tm_mon)], p->tm_mday,
            p->tm_hour, p->tm_min, p->tm_sec, 1900 + p->tm_year);

    return format;
}

// uid 转 所属用户名
static char *uid_to_name(uid_t uid)
{
    struct passwd * pw = getpwuid(uid);
    if (NULL == pw) {
        perror("passwd");
        return NULL;
    }
    return pw->pw_name;
}

// gid 转 所属组名
static char *gid_to_name(gid_t gid)
{
    struct group *grp = getgrgid(gid);
    if (NULL == grp) {
        perror("group");
        return NULL;
    }
    return grp->gr_name;
}