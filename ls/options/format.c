#include <string.h>

#include "format.h"
#include "long.h"
#include "../macro.h"


void format(const char * const *file_list, const char *real_path,
                                           char **fmt_files, int cnt)
{
    for (int i = 0; i < cnt; ++i) {
        char tmp[PATH_MAX_LEN + FILE_NAME_LEN];
        strcpy(tmp, real_path);
        strcat(tmp, file_list[i]);


        // 获取文件类型
        char type = get_type(tmp);

        switch (type)
        {
        // 软链接
        case 'l':
            strcpy(fmt_files[i], "@");
            break;
        // 文件夹
        case 'd':
            strcpy(fmt_files[i], "/");
            break;
        // 管道
        case 'p':
            strcpy(fmt_files[i], "|");
            break;
        default:
            if ('x' == get_mod(tmp)[2] || 'x' == get_mod(tmp)[5] ||
                                          'x' == get_mod(tmp)[8])
                strcpy(fmt_files[i], "*");
            break;
        }
    }
}


void format_single_file(const char *full_path, char *fmt_file)
{
    char type = get_type(full_path);

    switch (type)
    {
    // 软链接
    case 'l':
        strcpy(fmt_file, "@");
        break;
    // 文件夹
    case 'd':
        strcpy(fmt_file, "/");
        break;
    // 管道
    case 'p':
        strcpy(fmt_file, "|");
        break;
    default:
        if ('x' == get_mod(full_path)[2] || 'x' == get_mod(full_path)[5] ||
                                        'x' == get_mod(full_path)[8])
            strcpy(fmt_file, "*");
        break;
    }
}