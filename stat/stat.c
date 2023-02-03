#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <pwd.h> // struct passwd
#include <grp.h> // struct group
#include <time.h>

#include "../utils/utils.h"

// int stat(const char *pathname, struct stat *buf);
// int lstat(const char *pathname, struct stat *buf);


static const int usage_err = 1;

static const int getpwuid_failed = 2;
static const int getgrgid_failed = 4;

static const int stat_failed = 8;

static char get_type(const char *file_name);
static char* get_mod(const char *file_name);
static char *get_ower(const char *file_name);
static char *get_grp(const char *file_name);
static u_int32_t get_file_size(const char *file_name);
static char *get_time(const char *file_name);


static char *uid_to_name(uid_t uid);
static char *gid_to_name(gid_t gid);


int main(int argc, char **argv)
{
    exit_if(argc < 2, usage_err, stderr, "usage: %s <file>", argv[0])
    const char *file_name = argv[1];

    char file_type = get_type(file_name);
    printf("%c", file_type);

    char *file_mod = get_mod(file_name);
    printf("%s ", file_mod);

    char *ower = get_ower(file_name);
    printf("%s", ower);

    char *grp = get_grp(file_name);
    printf(" %s ", grp);

    u_int32_t fsize = get_file_size(file_name);
    printf("%.1lf KB", (fsize / 1024.0));

    char *format = get_time(file_name);
    printf(" %s", format);

    printf(" %s\n", file_name);

    free(format);
    format = NULL;
    free(file_mod);
    file_mod = NULL;

    return 0;
}


static char get_type(const char *file_name)
{
    struct stat myst;
    int ret = stat(file_name, &myst);
    exit_if(-1 == ret, stat_failed, stderr, "stat failed\n")

    char file_type;
    if(S_ISREG(myst.st_mode))
        file_type = '.';
    else if (S_ISDIR(myst.st_mode))
        file_type = 'd';
    else if (S_ISCHR(myst.st_mode))
        file_type = 'c';
    else if (S_ISBLK(myst.st_mode))
        file_type = 'b';
    else if (S_ISFIFO(myst.st_mode))
        file_type = 'p';
    else if (S_ISLNK(myst.st_mode))
        file_type = 'l';
    else if (S_ISSOCK(myst.st_mode))
        file_type = 's';
    else
        return '\0';

    return file_type;
}

static char* get_mod(const char *file_name)
{
    struct stat myst;
    int ret = stat(file_name, &myst);
    exit_if(-1 == ret, stat_failed, stderr, "stat failed\n")
    const int mode_size = 10;
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

static char *get_ower(const char *file_name)
{
    struct stat myst;
    int ret = stat(file_name, &myst);
    exit_if(-1 == ret, stat_failed, stderr, "stat failed\n")
    return uid_to_name(myst.st_uid);
}

static char *get_grp(const char *file_name)
{
    struct stat myst;
    int ret = stat(file_name, &myst);
    exit_if(-1 == ret, stat_failed, stderr, "stat failed\n")
    return gid_to_name(myst.st_gid);
}

static u_int32_t get_file_size(const char *file_name)
{
    struct stat myst;
    int ret = stat(file_name, &myst);
    exit_if(-1 == ret, stat_failed, stderr, "stat failed\n")
    return (u_int32_t)myst.st_size;
}

static char *get_time(const char *file_name)
{
    struct stat myst;
    int ret = stat(file_name, &myst);
    exit_if(-1 == ret, stat_failed, stderr, "stat failed\n")
    const char *wday[] = {"Sun", "Mon", "Tue", "Wed", "Thu", "Fri",
                            "Sat"};
    const char *ymoth[] = {"Jan", "Feb", "Mar", "Apr", "May", "Jun",
                            "Jul", "Aug", "Sep", "Oct", "Nov", "Dec"};
    time_t t = time(NULL); // 1970.1.1
    struct tm *p = localtime(&t);
    const int time_fmt_buf_size = 30;
    char *format = (char *)malloc(sizeof(char) * time_fmt_buf_size);
    sprintf(format, "%s %s %#2d %02d:%02d:%02d %d", 
            wday[p->tm_wday], ymoth[(p->tm_mon)], p->tm_mday,
            p->tm_hour, p->tm_min, p->tm_sec, 1900 + p->tm_year);

    return format;
}

static inline char *uid_to_name(uid_t uid)
{
    struct passwd * pw = getpwuid(uid);
    exit_if(NULL == pw, getpwuid_failed, stderr, "getpwuid failed\n")
    return pw->pw_name;
}

static inline char *gid_to_name(gid_t gid)
{
    struct group *grp = getgrgid(gid);
    exit_if(NULL == grp, getgrgid_failed, stderr, "getgrgid failed\n")
    return grp->gr_name;
}