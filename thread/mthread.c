//
// Created by fetch150zy on 1/8/23.
//

#include <sys/types.h>
#include <unistd.h> // Linux sys func (_exit())

#include <stdio.h>

#include <sys/wait.h> // for wait() waitpid()

// pid_t getpid(void);      get current process pid
// pit_t getppid(void);     get parent pid
// pid_t fork(void);        create a new process

void mfork(void);

void print_pid(void);

void run_another_exec(void);

void orphan_process(void);

void zombie_process(void);

void recycle_process(void);


int main(int argc, char **argv)
{
//    mfork();
//    print_pid();
//    run_another_exec();
//    orphan_process();
//    zombie_process();
    recycle_process();

//    puts("subprocess will not print this");

    return 0;
}

void mfork(void)
{
    pid_t pid = fork();
    if (pid > 0)
        printf("parent process: %d\n", getpid());
    else if (0 == pid)
        printf("subprocess: %d, parent process: %d\n", getpid(), getppid());
    else // pid == -1
        NULL;

    for (int i = 0; i < 5; ++i)
        printf("%d\n", i);
}

void print_pid(void)
{
    pid_t pid;
    for (int i = 0; i < 3; ++i) {
        pid = fork();
        if (0 == pid) // if subprocess, don't create
            break;
    }

    printf("current process pid: %d\n", getpid());
}

// use exec funcs, mainly using execl() execlp()
void run_another_exec(void)
{
    pid_t pid = fork();
    if (0 == pid) { // ml . -l
        printf("subprocess: %d\n", getpid());
        execl("../ls/ml", ".", "-l", NULL);
        _exit(0); // Linux sys func, end the subprocess
    } else if (pid > 0) {
        printf("parent process: %d\n", getpid());
    } else {
        NULL;
    }
}

void orphan_process(void)
{
    pid_t pid = fork();
    if (pid > 0) {
        printf("parent process: %d\n", getpid());
    } else if (0 == pid) {
        sleep(1); // sleep 1 second
        // new parent process
        printf("subprocess: %d, parent process: %d\n", getpid(), getppid());
    } else {
        NULL;
    }
}

// kill the parent process to solve this problem
void zombie_process(void)
{
    pid_t pid;
    // create subprocess
    for (int i = 0; i < 5; ++i) {
        pid = fork();
        if (0 == pid)
            break;
    }

    if (pid > 0) {
        // keep parent process running
        while (1) {
            printf("parent process: %d\n", getpid());
            sleep(1);
        }
    } else if (0 == pid) {
        printf("subprocess: %d, parent process: %d\n", getpid(), getppid());
    } else {
        NULL;
    }
}

// another way to solve the zombie process problem
// use wait() or waitpid()
void recycle_process(void)
{
//#define WAIT_PID_BLOCKING_MODE_

#ifdef WAIT_BLOCKING_MODE_
    // wait() blocking mode
    // pid_t wait(int *status);
    // man 2 wait
    pid_t pid;
    for (int i = 0; i < 5; ++i) {
        pid = fork();
        if (0 == pid)
            break;
    }

    if (pid > 0) {
        while (1) {
            pid_t subpid = wait(NULL);
            if (subpid > 0) {
                printf("recycle the resource of subprocess: %d\n", subpid);
            } else {
                puts("recycle filed, maybe no subprocess");
                break;
            }
            printf("i am parent process: %d\n", getpid());
        }
    } else if (0 == pid) {
        printf("i am subprocess: %d, parent process: %d\n", getpid(), getppid());
    } else {
        NULL;
    }

    // waitpid() non-blocking mode
    // pid_t waitpid(pid_t pid, int *status, int options);
    // man 2 waitpid

#elif defined(WAIT_PID_BLOCKING_MODE_)
    // blocking mode
    pid_t pid;
    for (int i = 0; i < 5; ++i) {
        pid = fork();
        if (0 == pid)
            break;
    }

    if (pid > 0) {
        while (1) {
            int status;
            pid_t subpid = waitpid(-1, &status, 0);
            if (subpid > 0) {
                printf("recycle the resource of subprocess: %d\n", subpid);
                if (WIFEXITED(status)) // normal exit
                    printf("status code: %d\n", WEXITSTATUS(status));
                if (WIFSIGNALED(status)) // kill by signal
                    printf("kill by signal: %d\n", WTERMSIG(status));
            } else {
                puts("recycle filed, maybe no subprocess");
                break;
            }
            printf("i am parent process: %d\n", getpid());
        }
    } else if (0 == pid) {
        printf("i am subprocess: %d, parent process: %d\n", getpid(), getppid());
    } else {
        NULL;
    }
#else
    // non-blocking mode
    pid_t pid;
    for (int i = 0; i < 5; ++i) {
        pid = fork();
        if (0 == pid)
            break;
    }

    if (pid > 0) {
        while (1) {
            int status;
            pid_t subpid = waitpid(-1, &status, WNOHANG);
            if (subpid > 0) {
                printf("recycle the resource of subprocess: %d\n", subpid);
                if (WIFEXITED(status)) // normal exit
                    printf("status code: %d\n", WEXITSTATUS(status));
                if (WIFSIGNALED(status)) // kill by signal
                    printf("kill by signal: %d\n", WTERMSIG(status));
            } else if (0 == subpid) {
                puts("subprocess has not exited");
            } else {
                puts("recycle filed, maybe no subprocess");
                break;
            }
            printf("i am parent process: %d\n", getpid());
        }
    } else if (0 == pid) {
        printf("i am subprocess: %d, parent process: %d\n", getpid(), getppid());
    } else {
        NULL;
    }
#endif
}