#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/mman.h>

#include "../utils/utils.h"

int main(void)
{
    int fd = open("src", O_RDWR);
    int size = lseek(fd, 0, SEEK_END);

    void* ptrA = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if(ptrA == MAP_FAILED)
        handle_error("mmap");

    int fd1 = open("dest", O_RDWR | O_CREAT, 0664);
    ftruncate(fd1, size);

    void* ptrB = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd1, 0);
    if(ptrB == MAP_FAILED)
        handle_error("mmap");

    memcpy(ptrB, ptrA, size);

    munmap(ptrA, size);
    munmap(ptrB, size);
    close(fd);
    close(fd1);

    return 0;
}