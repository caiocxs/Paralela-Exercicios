#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <stdint.h>

#define V_SIZE 4000000000
#define MAX_THREADS 4

typedef struct Parameter
{
    int *v;
    int max_iteration;
} Parameter;

void *callback_thread(void **arg)
{
    Parameter *p = (Parameter *)arg[0];
    int initial_i = arg[1];

    printf("FunctionCallBack - %d\n", p->i);

    int j = 0;
    while(j < p->max_iteration)
    {
      p->v[j + (initial_i * p->max_iteration)] = 2;
    }
}

extern int set_callback(void *arg)
{
    printf("SetCallBack\n");
    
    pthread_t id_thread;
    int i = 0;
    while (i < MAX_THREADS)
    {
        Parameter *p = (Parameter *)arg;
        
        pthread_create(&id_thread, NULL, callback_thread, [p, i]);
        i += 1;
    }
    pthread_join(id_thread, NULL);
}

int main(void)
{
    clock_t start, end;
    double cpu_time;

    int *parallel = (int *)malloc(V_SIZE);
    //int *sequential = (int *)malloc(V_SIZE);

    Parameter *p = (Parameter *)malloc(sizeof(Parameter));
    p->v = parallel;
    p->max_iteration = V_SIZE / MAX_THREADS;

    start = clock();
    set_callback(p);
    end = clock();

    cpu_time = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("\nFINISHED TIME: %f SECONDS\n", cpu_time);

    free(p);
    free(parallel);
    //free(sequential);

    return 0;
}
