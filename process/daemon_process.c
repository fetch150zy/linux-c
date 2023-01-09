//
// Created by fetch150zy on 1/9/23.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/time.h>
#include <time.h>


void write_file(int num);

int main(void)
{
    // create subprocess, kill the parent process
    pid_t pid = fork();
    if (pid > 0) // parent process
        exit(0);

    // subprocess to be the session
    setsid();

    // change work dir
    chdir("/home/fetch150zy");

    // set umask
    umask(022);

    // redirect fd about terminal
    int fd = open("/dev/null", O_RDWR);
    dup2(fd, STDIN_FILENO);
    dup2(fd, STDOUT_FILENO);
    dup2(fd, STDERR_FILENO);

    // catch signal
    struct sigaction act;
    act.sa_flags = 0;
    act.sa_handler = write_file;
    sigemptyset(&act.sa_mask);
    sigaction(SIGALRM, &act, NULL);

    // set time count
    struct itimerval val;
    val.it_value.tv_sec = 2;
    val.it_value.tv_usec = 0;
    val.it_interval.tv_sec = 2;
    val.it_interval.tv_usec = 0;
    setitimer(ITIMER_REAL, &val, NULL);

    while (1)
        sleep(100);

    return 0;
}

void write_file(int num)
{
    // get system time
    time_t seconds = time(NULL);
    // time trans
    struct tm* loc = localtime(&seconds);
    char *cur_time = asctime(loc);
    int fd = open("./time.log", O_WRONLY | O_CREAT | O_APPEND, 0664);
    write(fd, cur_time, strlen(cur_time));
    close(fd);
}