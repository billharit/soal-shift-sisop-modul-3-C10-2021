#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

#define X 4
#define Y 3
#define Z 6
#define T X*Z

int A[X][Y] = {
        {1, 1, 1},
        {1, 1, 1},
        {1, 1, 1},
        {1, 1, 1}
    },
    B[Y][Z] = {
        {1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1}
    },
    C[X][Z];

struct v 
{
    int i,
        j;
};

void *mult(void* arg)
{
    int i = ((struct v*)arg)->i,
        j = ((struct v*)arg)->j;

    for(int k=0; k<Y; k++)
    {
        C[i][j] += A[i][k] * B[k][j];
    }
    pthread_exit(0);
}


int main(int argc, char **argv){

    int i, j;
    int count = 0;

    key_t key = 1234;
    int *value;

    int shmid = shmget(key, sizeof(int)*T, IPC_CREAT | 0666);
    value = (int *)shmat(shmid, NULL, 0);

    pthread_t thr[T];

    for(i = 0; i < X; i++)
    {
        for(j = 0; j < Z; j++)
        {
            struct v *data = (struct v *) malloc(sizeof(struct v));
            data->i = i;
            data->j = j;

            pthread_create(&thr[count], NULL, mult, data);
            count++;
        }
    }

    for(i = 0; i < T; i++)
    {
        pthread_join(thr[i], NULL);
    }

    printf("Hasil perkalian matriks:\n");
    for(i = 0; i < X; i++)
    {
        for(j = 0; j < Z; j++)
        {
            value[i*Z + j] = C[i][j];
            printf("%d\t", value[i*Z +j]);
        }
        printf("\n");
    }


    shmdt(value);
    shmctl(shmid, IPC_RMID, NULL);
}