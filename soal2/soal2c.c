#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

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

    // char* argv[] = {"ps", "aux", NULL};
    //     execv("/bin/ps", argv);
    
    execlp("/bin/ps", "ps", "aux", NULL);
    
    perror("pipe failed");
    exit(1);
}

void exec2()
{
    dup2(fd1[0], 0);
    dup2(fd2[1], 1);

    closepipe1();
    closepipe2();

    // char* argv[] = {"sort", "-nrk", "3,3", NULL};
    //         execv("usr/bin/sort", argv);
    execlp("usr/bin/sort", "sort", "-nrk", "3,3", NULL);

    perror("pipe failed");
    exit(1);
}

void exec3()
{
    dup2(fd2[0], 0);

    closepipe2();

    // char* argv[] = {"head", "-5", NULL};
    //     execv("usr/bin/head", argv);
    
    execlp("usr/bin/head", "head", "-5", NULL);
    
    perror("pipe failed");
    exit(1);
}

int main()
{

    pipe(fd1);
    pipe(fd2);

    if(pipe(fd1) == -1)
        exit(1);
    if(pipe(fd2) == -1)
        exit(1);
    
    pid_t pid, pid1, pid2;
    
    pid = fork();

    if(pid < 0 )
    {
        fprintf(stderr, "fork failed");
        return 1;
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
}
