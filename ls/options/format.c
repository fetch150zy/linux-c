#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

#include "format.h"
#include "long.h"


void format(char **file_list, const char *full_path,
                              char **fmt_files,
                              int cnt)
{
    char *realp = realpath(full_path, NULL);
    for (int i = 0; i < cnt; ++i) {
        char tmp[100];
        strcpy(tmp, realp);
        strcat(tmp, "/");
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
            if ('x' == get_mod(tmp)[2] || 
                'x' == get_mod(tmp)[5] ||
                 'x' == get_mod(tmp)[8])
                strcpy(fmt_files[i], "*");
            break;
        }
    }
}