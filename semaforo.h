#include <pthread.h>

typedef struct Semaforo
{
    int valor;
    pthread_mutex_t mutex;
    pthread_cond_t condicao;
} Semaforo;

int in, out;
int running;

void semaforo_wait(Semaforo *s);
void semaforo_init(Semaforo *s, int value);
void semaforo_signal(Semaforo *s);
