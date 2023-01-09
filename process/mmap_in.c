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

void mmap_in(void);

int main(void)
{
    mmap_in();

    return 0;
}

void mmap_in(void)
{
    int fd = open("./memmap", O_RDONLY);
    void *addr = mmap(NULL, FOURK, PROT_READ, MAP_SHARED, fd, 0);
    if (MAP_FAILED == addr)
        handle_error("mmap");

    printf("%s", (char *) addr);

    munmap(addr, FOURK);
    close(fd);
}