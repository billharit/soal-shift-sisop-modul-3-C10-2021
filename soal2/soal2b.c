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
    new[M][N] 
    // = {
    //     {0, 1, 2, 3, 10, 5},
    //     {0, 1, 2, 3, 10, 5},
    //     {0, 1, 2, 3, 10, 5},
    //     {0, 1, 2, 3, 10, 5}
    // }
    ;

unsigned long long hasil[M][N];

struct v {
    int i,
        j;
};

unsigned long long factorial(unsigned long long n)
{
    if(n == 0)
        return 1;
    
    return n*factorial(n-1);
}

void *fact(void* arg)
{
    int i = ((struct v*)arg)->i,
        j = ((struct v*)arg)->j;
    
    int a = matrix[i][j],
        b = new[i][j];

    if(a == 0 || b == 0)
        hasil[i][j] = 0;
    else if(a >= b)
    {
        hasil[i][j] = factorial(a)/factorial(a-b);
    }
    else if(a < b)
        hasil[i][j] = factorial(a);
    
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

    printf("Masukkan matriks 4x6 :\n");
    for(i=0; i<M; i++)
    {
        for(j=0; j<N; j++)
        {
            scanf("%d", &new[i][j]);
        }
    }

    printf("matriks: \n");

    int a = 0;
    for(i=0; i<M; i++)
    {
        for(j=0; j<N; j++)
        {
            printf("%d\t", value[a]);
            matrix[i][j] = value[a];
            a++;
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

            pthread_create(&tid[count], NULL, &fact, (void *)data);
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

    shmdt(value);
    shmctl(shmid, IPC_RMID, NULL);

    return 0;
}