//
// Created by fetch150zy on 1/8/23.
//

#include <unistd.h>
#include <fcntl.h>

#include <stdio.h>
#include <string.h>

void fifo_in(void);

int main(void)
{
    fifo_in();

    return 0;
}

void fifo_in(void)
{
    // open pipe
    int rfd = open("./named", O_RDONLY);
    if (-1 == rfd) {
        perror("open");
        _exit(0);
    }
    puts("open pipe with read_only sucessfully");

    // read from pipe
    while (1) {
        char buf[1024];
        memset(buf, 0, sizeof(buf));
        if (read(rfd, buf, sizeof(buf)) == 0) {
            puts("write closed");
            break;
        }
        printf("%s", buf);
    }
    close(rfd);
}