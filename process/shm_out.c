//
// Created by fetch150zy on 1/9/23.
//

#include <stdio.h>
#include <sys/shm.h>
#include <string.h>
#include <stdlib.h>

#include "../utils/utils.h"

void shm_out(void);

int main(void)
{
    shm_out();

    return 0;
}

void shm_out(void)
{
    // create shared memory, 4K
    key_t key = ftok("/home/fetch150zy", 'm');
    int mode = 0664;
    int shmid = shmget(key, 4096, IPC_CREAT | mode);
    if (-1 == shmid)
        handle_error("shmget");

    // associated process and shared memory
    void *addr = shmat(shmid, NULL ,0);
    if ((void *)-1 == addr)
        handle_error("shmat");

    // write to shared memory
    const char *s = "this is shared-memory\n";
    memcpy(addr, s, strlen(s) + 1);

    // blocking
    puts("press any key to continue...");
    getchar();

    // disassociate
    shmdt(addr);

    // delete the shared memory
    shmctl(shmid, IPC_RMID, NULL);
    puts("shared-memory has been deleted");
}