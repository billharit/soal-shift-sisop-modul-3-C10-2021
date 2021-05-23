#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <errno.h>


int fd1[2];
int fd2[2];

void closepipe1()
{
    close(fd1[0]);
    close(fd1[1]);
}

void closepipe2()
{
    close(fd2[0]);
    close(fd2[1]);
}

void exec1()
{
    dup2(fd1[1], 1);
    closepipe1();

    char *argv1[] = {"ps", "aux", NULL};
    
    execv("/bin/ps", argv1);
    
    // execlp("/bin/ps", "ps", "aux", NULL);
    
    perror("ps aux failed");
    _exit(1);
}

void exec2()
{
    dup2(fd1[0], 0);
    dup2(fd2[1], 1);

    closepipe1();
    closepipe2();

    char *argv2[] = {"sort", "-nrk", "3,3", NULL};
    execv("/bin/sort", argv2);

    // execlp("usr/bin/sort", "sort", "-nrk", "3,3", NULL);

    perror("sort failed");
    _exit(1);
}

void exec3()
{
    dup2(fd2[0], 0);

    closepipe2();

    char *argv3[] = {"head", "-5", NULL};
    execv("/bin/head", argv3);
    
    // execlp("usr/bin/head", "head", "-5", NULL);
    
    perror("head failed");
    _exit(1);
}

int main()
{

    pipe(fd1);
    pipe(fd2);

    pid_t pid, pid1, pid2;
    

    if(pipe(fd1) == -1)
    {
        perror("Pipe 1 Creation is Failed\n");
        exit(EXIT_FAILURE);
    }

    if(pipe(fd2) == -1)
    {
        perror("Pipe 2 Creation is Failed\n");
        exit(EXIT_FAILURE);
    }
    
    pid = fork();

    if(pid < 0 )
    {
        fprintf(stderr, "fork failed");
        exit(EXIT_FAILURE);
    }
    else if(pid == 0)
    {
        exec1();
    }


    pid1 = fork();

    if(pid1 < 0 )
    {
        fprintf(stderr, "fork failed");
        return 1;
    }
    else if(pid1 == 0)
    {
        exec2();
    }

    closepipe1();

    pid2 = fork();
    if(pid2 < 0)
    {
        fprintf(stderr, "fork failed");
        return 1;
    }
    else if(pid2 == 0)
    {
        exec3();
    }

    exit(0);
}
