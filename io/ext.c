/**
 * ext.c (fetch150zy)
 * usage of lseek() truncate() ftruncate()
 * copyleft (c) 2023
*/

#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <assert.h>
#include <string.h>

#include "../utils/utils.h"


// #define EXT
#define EXT_

extern int errno;

static const int usage_err = 1;
static const int open_failed = 2;
static const int ftruncate_failed = 4;
static const int truncate_failed = 4;

off_t offset = 4095; // 4K

off_t ext(const char *file_name, int _4kb_size)
{
    int mode = 0664; // umask 022, mode & ~umask = 0644
    int fd = open(file_name, O_CREAT | O_RDWR | O_EXCL, mode);
    exit_if(-1 == fd, open_failed, stderr, "open %s failed.\n", file_name)

    off_t offt = 0;
    for (int i = 0; i < _4kb_size; ++i) {
        offt = lseek(fd, offset, SEEK_END) + 1l;
        write(fd, " ", 1);
    }

    close(fd);
    return offt;
}

off_t ext_(const char *file_name, int _4kb_size)
{
    int mode = 0664;
    int fd = open(file_name, O_CREAT | O_RDWR | O_EXCL, mode);
    exit_if(-1 == fd, open_failed, stderr, "open %s failed.\n", file_name)

    exit_if(-1 == ftruncate(fd, _4kb_size * offset), ftruncate_failed, stderr, "ext %s failed\n", file_name)
    // exit_if(-1 == truncate(file_name, _4kb_size * offset), truncate_failed, stderr, "ext %s failed\n", file_name)

    close(fd);
    return _4kb_size * (offset + 1l);
}

int main(int argc, char **argv)
{
    exit_if(argc < 3, usage_err, stderr, "usage: %s <filename> <n-4KB>\n", argv[0])

    const char *file_name = argv[1];
    int _4kb_size = atoi(argv[2]);
    exit_if(0 >= _4kb_size, usage_err, stderr, "<n-4KB> must be int\n")

#ifdef EXT
    off_t offt = ext(file_name, _4kb_size);
    printf("final write size = %ld\n", offt);
#elif defined(EXT_)
    off_t offt = ext_(file_name, _4kb_size);
    printf("final write size = %ld\n", offt);
#endif
        
    return 0;
}