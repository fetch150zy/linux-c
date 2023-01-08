#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h> // close(fd)
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

extern int errno;


#define buf_size 4096


int open_file(const char *file_name, const char mode)
{
    // 打开一个已经存在的磁盘文件
    // int open(const char *pathname, int flags);
    // 打开磁盘文件, 如果文件不存在, 就会自动创建
    // int open(const char *pathname, int flags, mode_t mode);

    int fd = -1;
    switch (mode)
    {
        case 'c':
            int mode = 0664; // umask 022, mode & ~umask = 0644
            // 创建权限为mode&~umask的文件并以读写方式打开，返回文件描述符
            fd = open(file_name, O_CREAT | O_RDWR, mode);
            break;
        case 'e':
            // 文件存在则创建失败，文件不存在则创建成功，返回文件描述符
            fd = open(file_name, O_CREAT | O_EXCL | O_RDWR);
            break;
        case 'o':
            // 以读写形式打开文件
            fd = open(file_name, O_RDWR);
            break;
        default:
            // 错误的命令格式
            printf("-c to create a new file and open it.\n"
                    "-o to just open it.\n"
                    "-e to check the file exist?\n");
            assert(0);
    }
    // 错误处理
    if (fd == -1) {
        fprintf(stderr, "cause by open(): %s\n", strerror(errno));
        assert(0);
    }
    return fd;
}

ssize_t read_file(int fd, void *buf)
{
    ssize_t size = read(fd, buf, buf_size);
    // 错误处理
    if (size == -1) {
        fprintf(stderr, "cause by read(): %s\n", strerror(errno));
        assert(0);
    // 未读完
    } else if (size > 0) {
        printf("read not over.\n");
    // 读完
    } else {
        printf("read over.\n");
    }
    return size;
}

ssize_t write_file(int fd, void *buf)
{
    ssize_t size = write(fd, buf, buf_size);
    // 错误处理
    if (size == -1) {
        fprintf(stderr, "cause by write(): %s\n", strerror(errno));
        assert(0);
    // 写完
    } else {
        printf("write over.\n");
    }
    return size;
}


int main(int argc, char **argv)
{
    // 调用形式不对
    if (argc < 3) {
        printf("usage: ./io filename -mode\n");
        return -1;
    }
    const char *file_name = argv[1];
    const char mode = argv[2][1];

    // 创建或打开文件
    int fd = open_file(file_name, mode);

    // 读写测试
    int buf[buf_size] = "hello linux.";
    ssize_t size = write_file(fd, buf);
    size = read_file(fd, buf);
    printf("%s\n", buf);

    // 关闭文件描述符
    close(fd);

    return 0;
}