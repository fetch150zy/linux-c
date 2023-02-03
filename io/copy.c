/**
 * copy.c (fetch150zy)
 * cp command???
 * copyleft (c) 2023
*/

#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <assert.h>

#include "../utils/utils.h"

static const int usage_err = 1;
static const int open_failed = 2;
static const int read_failed = 4;
static const int write_failed = 8;

typedef unsigned char byte;


static int copy_file(const char *src_file, const char *dest_file);

int main(int argc, char **argv)
{
    exit_if(argc < 3, usage_err, stderr, "usage: %s <src> <dest>\n", argv[0])

    const char *src_file = argv[1], *dest_file = argv[2];
    copy_file(src_file, dest_file);

    return 0;
}


int copy_file(const char *src_file, const char *dest_file)
{
    int src_fd = open(src_file, O_RDONLY);
    exit_if(-1 == src_fd, open_failed, stderr, "open %s failed\n", src_file)

    int mode = 0664;
    int dest_fd = open(dest_file, O_WRONLY | O_CREAT | O_EXCL, mode);
    exit_if(-1 == dest_fd, open_failed, stderr, "open %s failed\n", dest_file)

    const size_t buf_size = 4096;
    byte buf[buf_size];
    size_t size = 0;
    while ((size = read(src_fd, buf, buf_size)) > 0)
        exit_if(-1 == write(dest_fd, buf, size), write_failed, stderr, "write %s failed\n", dest_file)
    exit_if(-1 == size, read_failed, stderr, "read %s failed\n", src_file)    
        
    close(src_fd);
    close(dest_fd);
    return 0;
}
