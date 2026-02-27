#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <stdint.h>

#define BUFFER_SIZE 10

typedef struct Semaforo
{
    int valor;
    pthread_mutex_t mutex;
    pthread_cond_t condicao;
} Semaforo;

Semaforo mutex;
Semaforo vazio;
Semaforo cheio;

int buffer[BUFFER_SIZE] = {};
int in = 0, out = 0;
int running = 1;

int GetBufferPosition(int empty)
{
    for (int i = 0; i < BUFFER_SIZE; i++)
    {
        if (empty == 0 && buffer[i] > 0)
            return i;
        else if (empty == 1 && buffer[i] == 0)
            return i;
    }
}

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

void *Produtor(void *arg)
{
    while(running){
        int data = rand() % 8 + 1;
        wait(&vazio);
        wait(&mutex);
        buffer[in] = data;
        in = (in + 1) % BUFFER_SIZE;
        printf("\nDado produzido: %d", data);
        signal(&mutex);
        signal(&cheio);
    }
    return NULL;
}

void *Consumidor(void *arg)
{
    while(running){
        wait(&cheio);
        wait(&mutex);
        int data = buffer[out];
        out = (out + 1) % BUFFER_SIZE;
        signal(&mutex);
        signal(&vazio);
        printf("\nDado consumido: %d", data);
    }
    return NULL;
}

int main(void)
{
    srand(time(NULL));
    mutex.valor = 1; // 1 - open; 0 - closed
    vazio.valor = BUFFER_SIZE; // how many free spots on buffer
    cheio.valor = 0; // 1 - full; 0 - there are free spots

    pthread_t t_produtor;
    pthread_t t_consumidor;

    pthread_create(&t_produtor, NULL, Produtor, NULL);
    pthread_create(&t_consumidor, NULL, Consumidor, NULL);
    
    sleep(0.7f);

    pthread_mutex_lock(&mutex.mutex);
    running = 0;
    pthread_mutex_unlock(&mutex.mutex);

    pthread_cond_broadcast(&vazio.condicao);
    pthread_cond_broadcast(&cheio.condicao);

    pthread_join(t_produtor, NULL);
    pthread_join(t_consumidor, NULL);

    return 0;
}