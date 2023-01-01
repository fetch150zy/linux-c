#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>

typedef unsigned char byte;

int copy_file(const char *src_file, const char *dest_file);

int main(int argc, char **argv)
{
    if (argc < 3) {
        printf("usage: ./copy src_file dest_file\n");
        return -1;
    }
    const char *src_file = argv[1], *dest_file = argv[2];
    int errf = copy_file(src_file, dest_file);
    if (errf == -1)
        assert(0);

    return 0;
}

int copy_file(const char *src_file, const char *dest_file)
{
    // 只读模式打开原文件
    int src_fd = open(src_file, O_RDONLY);
    if (src_fd == -1) {
        perror("open read-file");
        return -1;
    }

    // 创建并以只写模式打开目标文件
    int mode = 0664;
    int dest_fd = open(dest_file, O_WRONLY | O_CREAT, mode);
    if (src_fd == -1) {
        perror("open write-file");
        return -1;
    }

    // 拷贝数据
    const size_t buf_size = 4096;
    byte buf[buf_size];
    size_t size = -1;
    while ((size = read(src_fd, buf, buf_size)) > 0)
        write(dest_fd, buf, size);
    // 关闭文件描述符
    close(src_fd);
    close(dest_fd);
    return 0;
}
