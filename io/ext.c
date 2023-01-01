#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <assert.h>
#include <string.h>


extern int errno;

off_t offset = 4095; // 4K

off_t ext(const char *file_name, int kb_size)
{
    int mode = 0664; // umask 022, mode & ~umask = 0644
    // 创建权限为mode&~umask的文件并以读写方式打开，返回文件描述符
    int fd = open(file_name, O_CREAT | O_RDWR, mode);
    if (-1 == fd) {
        fprintf(stderr, "cause by open(): %s\n", strerror(errno));
        assert(0);
    }

    off_t offt = 0;
    for (int i = 0; i < kb_size; ++i) {
        offt += lseek(fd, offset, SEEK_END);
        write(fd, " ", 1);
    }

    // use ftruncate and truncate
    // int truncate(const char *path, off_t length);
    // int ftruncate(int fd, off_t length);

    close(fd);
    return offt;
}

int main(int argc, char **argv)
{
    if (argc < 3) {
        printf("usage: ./ext filename n-KB\n");
        return -1;
    }
    const char *file_name = argv[1];
    int kb_size = atoi(argv[2]);
    off_t offt = ext(file_name, kb_size);
    printf("final write size = %zd\n", offt);
        
    return 0;
}