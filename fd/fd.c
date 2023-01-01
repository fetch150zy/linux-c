#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <fcntl.h>
#include <unistd.h>

extern int errno;

int get_file_fd(const char *file_name);
int copy_fd_dup(int oldfd);
int change_fd_fcntl(int oldfd);
int get_file_flags(int fd);
void set_file_flag(int fd, int flag);
int redirect_fd(int oldfd, int newfd);

int main(int argc, char **argv)
{
    if (argc < 2) {
        perror("usage: ./fd filename\n");
        return -1;
    }
    const char *file_name = argv[1];
    int fd = get_file_fd(file_name);
    printf("the fd of %s is: %d\n", file_name, fd);
    int newfd = copy_fd_dup(fd);
    printf("the newfd of %s is: %d\n", file_name, newfd);
    close(newfd);
    newfd = change_fd_fcntl(fd);
    printf("the newfd of %s is: %d\n", file_name, newfd);
    int flag = get_file_flags(newfd);
    flag = flag | O_WRONLY; // 加上写
    set_file_flag(newfd, flag);

    return 0;
}

int get_file_fd(const char *file_name)
{
    int mode = 0664; // 0664 & ~0022 = 0644
    int fd = open(file_name, O_RDONLY | O_CREAT, mode);
    if (-1 == fd) {
        fprintf(stderr, "cause by open(): %s\n",
                strerror(errno));
        assert(0);
    }
    return fd;
}

// use dup()
// 拷贝文件描述符
int copy_fd_dup(int oldfd)
{
    int newfd = dup(oldfd);
    if (-1 == newfd) {
        fprintf(stderr, "cause by dup(): %s\n",
                strerror(errno));
        assert(0);
    }
    return newfd;
}

// use fcntl()
// 改变文件的文件描述符 oldfd -> newfd
int change_fd_fcntl(int oldfd)
{
    // F_DUPFD F_GETFL F_SETFL
    int newfd = fcntl(oldfd, F_DUPFD);
    close(oldfd);
    return newfd;
}

// 获取文件状态标志
int get_file_flags(int fd)
{
    int flag = fcntl(fd, F_GETFL);
    return flag;
}

// 设置文件状态标志
void set_file_flag(int fd, int flag)
{
    fcntl(fd, F_SETFL, flag);
}

// use dup2()
// 文件描述符重定向
int redirect_fd(int oldfd, int newfd)
{
    // 最终 newfd 和 oldfd 都将指向oldfd指向的文件
    int fd = dup2(oldfd, newfd);
    if (-1 == fd) {
        fprintf(stderr, "cause by dup2(): %s\n",
                strerror(errno));
        assert(0);
    }
    return fd;
}