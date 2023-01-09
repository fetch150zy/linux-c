//
// Created by fetch150zy on 1/9/23.
//

#include <sys/mman.h>
#include <unistd.h>
#include <fcntl.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../include/macro.h"

#define FOURK 4096

void mmap_out(void);

int main(void)
{
    mmap_out();

    return 0;
}

void mmap_out(void)
{
    int fd = open("./memmap", O_RDWR);
    void *addr = mmap(NULL, FOURK, PROT_WRITE, MAP_SHARED, fd, 0);
    if (MAP_FAILED == addr)
        handle_error("mmap");

    for (int i = 0; i < 100; ++i) {
        char s[128];
        sprintf(s, "%d: i am parent process\n", i);
        memcpy(addr, s, strlen(s) + 1);
        sleep(1);
    }

    munmap(addr, FOURK);
    close(fd);
}