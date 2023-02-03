//
// Created by fetch150zy on 1/9/23.
//

#include <stdio.h>
#include <sys/shm.h>
#include <string.h>
#include <stdlib.h>

#include "../utils/utils.h"

void shm_in(void);

int main(void)
{
    shm_in();

    return 0;
}

void shm_in(void)
{
    // open the shared-memory
    key_t key = ftok("/home/fetch150zy", 'm');
    int shmid = shmget(key, 0, 0);
    if (-1 == shmid)
        handle_error("shmget");

    // associated process and shared memory
    void *addr = shmat(shmid, NULL ,0);
    if ((void *)-1 == addr)
        handle_error("shmat");

    // read from shared-memory
    printf("%s", (char *)addr);

    // blocking
    puts("press any key to continue...");
    getchar();

    // disassociate
    shmdt(addr);

    // delete the shared-memory
    shmctl(shmid, IPC_RMID, NULL);
    puts("shared-memory has been deleted");
}