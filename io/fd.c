/**
 * fd.c (fetch150zy)
 * usage of file descriptor
 * copyleft (c) 2023
*/

#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include <fcntl.h>

#include "../utils/utils.h"

extern int errno;

static const int usage_failed = 1;
static const int open_failed = 2;
static const int dup_failed = 4;
static const int dup2_failed = 8;
static const int fcntl_failed = 16;

static int get_file_descriptor(const char *file_name);
static int copy_fd_dup(int oldfd);
static int change_fd_fcntl(int oldfd);
static int get_file_flags(int fd);
static void set_file_flag(int fd, int flag);
static int redirect_fd(int oldfd, int newfd);


int main(int argc, char **argv)
{
    exit_if(argc < 2, usage_failed, stderr, "usage: ./%s filename\n", argv[1])

    const char *file_name = argv[1];
    int fd = get_file_descriptor(file_name);
    printf("the fd of %s is: %d\n", file_name, fd);
    
    // now the new fd and the old fd ==> file
    int newfd = copy_fd_dup(fd);
    printf("the newfd of %s is: %d\n", file_name, newfd);
    close(newfd);

    newfd = change_fd_fcntl(fd);
    printf("the newfd of %s is: %d\n", file_name, newfd);

    int flag = get_file_flags(newfd);
    flag = flag | O_WRONLY; 
    set_file_flag(newfd, flag);

    return 0;
}


static int get_file_descriptor(const char *file_name)
{
    int mode = 0664; // 0664 & ~0022 = 0644
    int fd = open(file_name, O_RDONLY | O_CREAT | O_EXCL, mode);
    exit_if(-1 == fd, open_failed, stderr, "open %s failed\n", file_name)
    return fd;
}


// use dup()
static int copy_fd_dup(int oldfd)
{
    int newfd = dup(oldfd);
    exit_if(-1 == newfd, dup_failed, stderr, "dup failed\n")
    return newfd;
}

// use fcntl() oldfd -> newfd
static int change_fd_fcntl(int oldfd)
{
    // F_DUPFD F_GETFL F_SETFL
    int newfd = fcntl(oldfd, F_DUPFD);
    close(oldfd);
    return newfd;
}

// get file status
static int get_file_flags(int fd)
{
    int flag = fcntl(fd, F_GETFL);
    return flag;
}

// set file status 
static void set_file_flag(int fd, int flag)
{
    fcntl(fd, F_SETFL, flag);
}

// use dup2()
static int redirect_fd(int oldfd, int newfd)
{
    // newfd oldfd ==> oldfd
    int fd = dup2(oldfd, newfd);
    exit_if(-1 == fd, dup2_failed, stderr, "dup2 failed\n")
    return fd;
}