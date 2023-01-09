//
// Created by fetch150zy on 1/8/23.
//

#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>

#include <string.h>
#include <stdio.h>

// read from pipe
//ssize_t read(int fd, void *buf, size_t count);
// write to pipe
//ssize_t write(int fd, const void *buf, size_t count);

void anonymous_pipe(void);

int main(void)
{
    anonymous_pipe();

    return 0;
}

void anonymous_pipe(void)
{
    // int pipe(int pipefd[2])
    // pipefd[0] read, pipefd[1] write

    // create anonymous pipe
    int fd[2];
    int ret = pipe(fd);
    if (-1 == ret) {
        perror("pipe");
        _exit(0);
    }

    // set read and write as non-block (not suggest)
    int flag = fcntl(fd[0], F_GETFL);
    flag |= O_NONBLOCK;
    fcntl(fd[0], F_SETFL, flag);
    fcntl(fd[1], F_SETFL, flag);

    // create subprocess
    pid_t pid = fork();
    if (0 == pid) { // subprocess
        // close read
        close(fd[0]);
        // redirect
        dup2(fd[1], STDOUT_FILENO);
        // run ml -l
        execl("../ls/ml", "ml", "/", "-l", "-f", NULL);
        perror("execl");
    } else if (pid > 0) { // parent process
        // close write
        close(fd[1]);
        char buf[4096];
        while (1) {
            memset(buf, 0, sizeof(buf));
            int len = read(fd[0], buf, sizeof(buf));
            if (0 == len)
                break;
            printf("%s", buf);
        }
        close(fd[0]);

        wait(NULL);
    } else {
        perror("fork");
    }
}

