#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>

#include <string.h>
#include <stdbool.h>

#include "list.h"

#define path_num 128
#define op_num 128
#define path_max_len 40
#define op_max_len 10

char **ml(const char *path, byte *cnt, bool *has_ops);
void list(const char *file_name);
void usage(int status);

int main(int argc, char **argv)
{
    // 存 path 和 op
    char **path = (char **)malloc(sizeof(char *) * path_num);
    char **ops = (char **)malloc(sizeof(char *) * op_num);
    // path 和 op 的数目
    byte pcnt = 0, ocnt = 0;
    // 有传参或者有传入操作符
    if (argc > 1) {
        // 解析参数和操作符（参数和操作符位置无硬性要求）
        for (int i = 1; i < argc; ++i) {
            if ('-' != argv[i][0]) {
                path[pcnt] = (char *)malloc(sizeof(char) * path_max_len);
                strcpy(path[pcnt++], argv[i]);
            } else {
                ops[ocnt] = (char *)malloc(sizeof(char) * op_max_len);
                strcpy(ops[ocnt++], argv[i]);
            }
        }
    }
    // 不传入参数时默认当前目录
    if (1 == argc || 0 == pcnt) {
        path[pcnt] = (char *)malloc(sizeof(char) * 2);
        strcpy(path[pcnt++], ".");
    }

    // 0: -a    1: -l   2: -h
    bool has_ops[3] = {false, false, false};
    for (int i = 0; i < ocnt; ++i) {
        if (0 == strcmp(ops[i], "-a") ||
                0 == strcmp(ops[i], "--all"))
            has_ops[0] = true;
        if (0 == strcmp(ops[i], "-l") ||
                0 == strcmp(ops[i], "--long"))
            has_ops[1] = true;
        if (0 == strcmp(ops[i], "-h") ||
                0 == strcmp(ops[i], "--help"))
            has_ops[2] = true;
    }

    for (int i = 0; i < pcnt; ++i) {
        byte cnt = 0;
        char **file_list = ml(path[i], &cnt, has_ops);
        if (has_ops[1]) {
            for (int j = 0; j < cnt; ++j) 
                list(file_list[j]);
        } else {
            for (int j = 0; j < cnt; ++j) {
                if ((j + 1) % 7 == 0)
                    putchar('\n');
                printf(" %s ", file_list[j]);
            }
            putchar('\n');
        }

        for (int i = 0; i < cnt; ++i)
            free(file_list[i]);
        free(file_list);
        file_list = NULL;
    }

    for (int i = 0; i < pcnt; ++i)
        free(path[i]);
    free(path);
    path = NULL;

    for (int i = 0; i < ocnt; ++i)
        free(ops[i]);
    free(ops);
    ops = NULL;

    return 0;
}

char **ml(const char *path, byte *cnt, bool *has_ops)
{
    // opendir
    DIR *dir = opendir(path);
    if (NULL == dir) {
        fprintf(stderr, "cause by opendir(): %s\n",
                    strerror(errno));
        assert(0);
    }

    // readdir
    const byte file_list_size = 255;
    const byte file_name_length = 20;
    char **file_list = (char **)malloc(sizeof(char *) * file_list_size);

    // -a
    byte i = 0;
    if (has_ops[0]) {
        struct dirent *d = NULL;
        while ((d = readdir(dir)) != NULL) {
            file_list[i] = (char *)malloc(sizeof(char) * file_name_length);
            strcpy(file_list[i++], d->d_name);
            ++(*cnt);
        }
    } else {
        struct dirent *d = NULL;
        while ((d = readdir(dir)) != NULL) {
            if ('.' != d->d_name[0]) {
                file_list[i] = (char *)malloc(sizeof(char) * file_name_length);
                strcpy(file_list[i++], d->d_name);
                ++(*cnt);
            }
        }
    }

    // -h
    if (has_ops[2]) {
        usage(0);
    }

    // closedir
    int err_ = closedir(dir);
    if (-1 == err_) {
        fprintf(stderr, "cause by closedir(): %s\n",
                    strerror(errno));
        assert(0);
    }

    return file_list;
}

void list(const char *file_name)
{
    char file_type = get_type(file_name);
    printf("\033[38;5;7m");
    printf("%c", file_type);

    char *file_mod = get_mod(file_name);
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

    char *ower = get_ower(file_name);
    printf("\033[38;5;7m");
    printf("%s", ower);

    char *grp = get_grp(file_name);
    printf("\033[38;5;239m");
    printf(" %s ", grp);

    u_int32_t fsize = get_file_size(file_name);
    printf("\033[38;5;228m");
    printf("%.1lf KB", (fsize / 1024.0));

    char *format = get_time(file_name);
    printf("\033[38;5;40m");
    printf(" %s", format);

    printf("\033[38;5;56m");
    printf(" %s\n", file_name);

    free(format);
    format = NULL;
    free(file_mod);
    file_mod = NULL;
}

void usage(int status)
{
    printf("\033[38;5;46m");
    printf("ml");
    printf("\033[38;5;253m");
    printf(" 0.23.1");
    printf("\nA ls command with colors.\n\n");

    printf("\033[38;5;190m");
    printf("USAGE:\n");
    printf("\033[38;5;253m");
    printf("    ml [OPTIONS] [FILE]...\n\n");

    printf("\033[38;5;190m");
    printf("ARGS:\n");
    printf("\033[38;5;154m");
    printf("    <FILE>...");
    printf("\033[38;5;253m");
    printf("   [default: .]\n\n");

    printf("\033[38;5;190m");
    printf("OPTIONS:\n");

    char ops[3][11] = {
        "-a, --all",
        "-l, --long",
        "-h, --help"
    };
    char ops_detail[3][50] = {
        "Do not ignore entries starting with .",
        "Display extended file metadata as a table .",
        "Show you the help info about ml ."
    };
    for (int i = 0; i < 3; ++i) {
        printf("\033[38;5;118m");
        printf("    %-18s", ops[i]);
        printf("\033[38;5;253m");
        printf("%s\n", ops_detail[i]);
    }

    exit(status);
}