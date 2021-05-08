#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>

#define M 4
#define N 6
#define T M*N

int matrix[M][N],
    new[M][N] = {
        {1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1}
    };

unsigned long long hasil[M][N];

struct v {
    int i,
        j;
};

void *fact(void* arg)
{
    int i = ((struct v*)arg)->i,
        j = ((struct v*)arg)->j;

    unsigned long long angkaA = 1,
                        angka = 1;
    
    int a = matrix[i][j],
        b = new[i][j];

    for(int k = a; k > 0 ; k--)
        angkaA*=(unsigned long long)k;

    if(a == 0 || b == 0)
        hasil[i][j] = 0;
    else if(a >= b)
    {
        for(int k = a-b; k > 0 ; k--)
            angka*=(unsigned long long)k;
        
        hasil[i][j] = angkaA/angka;
    }
    else if(a < b)
        hasil[i][j] = angkaA;
    
    pthread_exit(0);
}

int main()
{
    int i, j;

    key_t key = 1234;
    int *value;
    int shmid = shmget(key, sizeof(int)*T, IPC_CREAT | 0666);
    value = (int *)shmat(shmid, NULL, 0);

    pthread_t tid[T];

    printf("matriks: \n");
    for(i=0; i<M; i++)
    {
        for(j=0; j<N; j++)
        {
            printf("%d\t", value[i*N + j]);
            matrix[i][j] = value[i*N + j];
        }
        printf("\n");
    }

    int count = 0;
    //hasil factorial
    for(i=0; i<M; i++)
    {
        for(j=0; j<N; j++)
        {
            struct v *data = (struct v*)malloc(sizeof(struct v));
            data->i = i;
            data->j = j;

            pthread_create(&tid[count], NULL, fact, (void *)data);
            count++;
        }
    }

    for(i=0; i<T; i++)
    {
        pthread_join(tid[i], NULL);
    }

    printf("Hasil matriks: \n");
    for(i=0; i<M; i++)
    {
        for(j=0; j<N; j++)
        {
            printf("%llu\t", hasil[i][j]);
        }
        printf("\n");
    }

    shmdt (value);
    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}