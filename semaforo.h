#include <pthread.h>

typedef struct Semaforo
{
    int valor;
    pthread_mutex_t mutex;
    pthread_cond_t condicao;
} Semaforo;

int in, out;
int running;

void wait(Semaforo *s);

void signal(Semaforo *s);
