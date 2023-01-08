//
// Created by fetch150zy on 1/8/23.
//

#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#include <string.h>
#include <stdio.h>

void fifo_out(void);

int main(void)
{
    fifo_out();

    return 0;
}

// fifo
void fifo_out(void)
{
    // create named pipe (use mkfifo)
    // int mkfifo(const char *pathname, mode_t mode)
    int mode = 0664;
    int ret = mkfifo("./named", mode);
    if (-1 == ret) {
        perror("mkfifo");
        _exit(0);
    }
    puts("create pipe sucessfully");

    // open pipe
    int wfd = open("./named", O_WRONLY);
    if (-1 == wfd) {
        perror("open");
        _exit(0);
    }
    puts("opne pipe with write_only sucessfully");

    // write to pipe
    for (int i = 0; i < 100; ++i) {
        char buf[1024];
        sprintf(buf, "i am writing to pipe: %d\n", i);
        write(wfd, buf, strlen(buf));
        sleep(1);
    }
    close(wfd);
}