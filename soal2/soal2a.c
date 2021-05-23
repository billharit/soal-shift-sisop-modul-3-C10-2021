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

int A[X][Y] 
    
        // = {
// 4, 1, 4,
// 2, 1, 3,
// 4, 2, 2,
// 1, 1, 4
    // }
    ,
    B[Y][Z] 
//         = {
// 2, 1, 3, 2, 0, 3,
// 1, 4, 4, 0, 0, 2,
// 1, 1, 0, 1, 2, 1
//     }
    ,
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

    int *p = (int *) malloc(sizeof(int));
    *p = C[i][j];

    pthread_exit(p);
}


int main(){

    int i, j;
    int count = 0;

    key_t key = 1234;
    int *value;

    int shmid = shmget(key, sizeof(int)*T, IPC_CREAT | 0666);
    value = (int *)shmat(shmid, NULL, 0);

    pthread_t thr[T];

    printf("Masukkan matriks A :\n");
    for(i = 0; i < X; i++)
    {
        for(j = 0; j < Y; j++)
        {
            scanf("%d", &A[i][j]);
        }
    }

    printf("Masukkan matriks B :\n");
    for(i = 0; i < Y; i++)
    {
        for(j = 0; j < Z; j++)
        {
            scanf("%d", &B[i][j]);
        }
    }

    count = 0;
    for(i = 0; i < X; i++)
    {
        for(j = 0; j < Z; j++)
        {
            struct v *data = (struct v *) malloc(sizeof(struct v));
            data->i = i;
            data->j = j;

            pthread_create(&thr[count], NULL, &mult, (void *)data);
            count++;
        }
    }

    for(i = 0; i < T; i++)
    {
        pthread_join(thr[i], NULL);
    }

    int a = 0;
    printf("Hasil perkalian matriks:\n");
    for(i = 0; i < X; i++)
    {
        for(j = 0; j < Z; j++)
        {
            value[a] = C[i][j];
            printf("%d\t", value[a]);
            a++;
        }
        printf("\n");
    }

    shmdt(value);
    // shmctl(shmid, IPC_RMID, NULL);
    // sleep(50);
}