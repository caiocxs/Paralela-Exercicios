#define _POSIX_C_SOURCE 200112L

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <stdint.h>

#define V_SIZE 4000000000
#define MAX_THREADS 4


typedef struct ArrayData {
    int8_t *v;
    long sum;
} ArrayData;

typedef struct ThreadArgs
{
    ArrayData *arr;
    int i;
    int isPopulated;
} ThreadArgs;

void *callback_thread(void *arg)
{
    ThreadArgs *args = (ThreadArgs *)arg;
    int8_t *v = args->arr->v;
    int i = args->i;
    long j = 0;

    long max_ite = V_SIZE / MAX_THREADS;
    if (!args->isPopulated)
    {
        unsigned int local_seed = time(NULL) ^ i;

        while (j < max_ite)
        {
            v[j + i * max_ite] = rand_r(&local_seed) % 8 + 1;
            j += 1;
        }
    }
    else
    {
        long sum = 0;
        while (j < max_ite)
        {
            sum += v[j + i * max_ite];
            j += 1;
        }

        __sync_fetch_and_add(&args->arr->sum, sum);
    }

    free(args);
}

extern int set_callback(void *arg)
{
    printf("SetCallBack\n");

    pthread_t threads[MAX_THREADS];
    int i = 0;

    while (i < MAX_THREADS)
    {
        ThreadArgs *args = (ThreadArgs *)malloc(sizeof(ThreadArgs));
        args->arr = (ArrayData *)arg;
        args->i = i;
        args->isPopulated = 0;
        pthread_create(&threads[i], NULL, callback_thread, args);
        i += 1;
    }

    i = 0;
    while (i < MAX_THREADS)
    {
        pthread_join(threads[i], NULL);
        i += 1;
    }

    i = 0;
    while (i < MAX_THREADS)
    {
        ThreadArgs *args = (ThreadArgs *)malloc(sizeof(ThreadArgs));
        args->arr = (ArrayData *)arg;
        args->i = i;
        args->isPopulated = 1;
        pthread_create(&threads[i], NULL, callback_thread, args);
        i += 1;
    }

    i = 0;
    while (i < MAX_THREADS)
    {
        pthread_join(threads[i], NULL);
        i += 1;
    }

    return 0;
}

int main(void)
{
    srand(time(NULL));
    
    struct timespec start, end;
    double real_time;

    int8_t *parallel = (int8_t *)malloc(sizeof(int8_t) * V_SIZE);
    if (parallel == NULL)
    {
        printf("\nErro ao alocar");
        return -1;
    }
    int8_t *sequential = (int8_t *)malloc(sizeof(int8_t) * V_SIZE);
    if (sequential == NULL)
    {
        printf("\nErro ao alocar");
        return -1;
    }

    ArrayData *arr_p = (ArrayData *)malloc(sizeof(ArrayData));
    arr_p->v = parallel;
    arr_p->sum = 0;
    ArrayData *arr_s = (ArrayData *)malloc(sizeof(ArrayData));
    arr_s->v = sequential;
    arr_s->sum = 0;

    clock_gettime(CLOCK_MONOTONIC, &start);
    set_callback(arr_p);
    clock_gettime(CLOCK_MONOTONIC, &end);

    real_time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1000000000.0;
    printf("\nTEMPO REAL (PARALLEL): %f SEGUNDOS\n", real_time);

    clock_gettime(CLOCK_MONOTONIC, &start);
    for (long i = 0; i < V_SIZE; i++)
    {
        arr_s->v[i] = rand() % 8 + 1;
    }
    for (long i = 0; i < V_SIZE; i++)
    {
        arr_s->sum += arr_s->v[i];
    }
    clock_gettime(CLOCK_MONOTONIC, &end);

    real_time = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1000000000.0;
    printf("\nTEMPO REAL (SEQUENTIAL): %f SEGUNDOS\n", real_time);

    free(parallel);
    free(sequential);

    return 0;
}
