//
// Created by fetch150zy on 1/9/23.
//

#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../include/macro.h"

#define FOURK 4096

// cretae mmap
//void *mmap(void *addr, size_t length, int prot, int flags, int fd, off_t offset);

void mem_map(void);

int main(void)
{
    mem_map();

    return 0;
}

void mem_map(void)
{
    // open the disk file
    int fd = open("./memmap", O_RDWR);
    // create memory map
    void *addr = mmap(NULL, FOURK, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (MAP_FAILED == addr)
        handle_error("mmap");

    // create subprocess
    pid_t pid = fork();
    if (pid > 0) { // parent process
        const char *s = "i am parent process";
        memcpy(addr, s, strlen(s) + 1);
        sleep(1);
    } else if (0 == pid) { // subprocess
        printf("%s\n", (char *)addr);
    } else {
        handle_error("fork");
    }

    munmap(addr, FOURK);
    close(fd);
}

