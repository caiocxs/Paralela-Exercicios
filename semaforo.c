#include <pthread.h>

typedef struct Semaforo
{
    int valor;
    pthread_mutex_t mutex;
    pthread_cond_t condicao;
} Semaforo;

void wait(Semaforo *s)
{
    pthread_mutex_lock(&s->mutex);
    while (s->valor <= 0)
    {
        pthread_cond_wait(&s->condicao, &s->mutex);
    }
    s->valor--;
    pthread_mutex_unlock(&s->mutex);
}

void signal(Semaforo *s)
{
    pthread_mutex_lock(&s->mutex);
    s->valor++;
    pthread_cond_signal(&s->condicao);
    pthread_mutex_unlock(&s->mutex);
}