#include <pthread.h>

typedef struct Semaforo
{
    int valor;
    pthread_mutex_t mutex;
    pthread_cond_t condicao;
} Semaforo;

void semaforo_wait(Semaforo *s)
{
    pthread_mutex_lock(&s->mutex);
    while (s->valor <= 0)
    {
        pthread_cond_wait(&s->condicao, &s->mutex);
    }
    s->valor--;
    pthread_mutex_unlock(&s->mutex);
}

void semaforo_signal(Semaforo *s)
{
    pthread_mutex_lock(&s->mutex);
    s->valor++;
    pthread_cond_signal(&s->condicao);
    pthread_mutex_unlock(&s->mutex);
}

void semaforo_init(Semaforo *s, int value) {
    s->valor = value;
    pthread_mutex_init(&s->mutex, NULL);
    pthread_cond_init(&s->condicao, NULL);
}