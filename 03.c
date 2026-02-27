#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <stdint.h>
#include "semaforo.h"

Semaforo MulherDisponivel;
Semaforo HomemDisponivel;
int esperando = 0; // -1 - mulher; 1 - homem
int idH = 1, idM = 1;

void *ChegarFestaHomem(void *arg)
{
    while (running)
    {
        signal(&HomemDisponivel);
        printf("\nChegou um homem id: %d!", idH);
        idH++;
        wait(&MulherDisponivel);
        printf("Foi feito um casal!");
    }
}

void *ChegarFestaMulher(void *arg)
{
    while (running)
    {
        signal(&MulherDisponivel);
        printf("\nChegou uma mulher id: %d!", idM);
        idM++;
        wait(&HomemDisponivel);
        printf("Foi feito um casal!");
    }
}

int main(void)
{
    HomemDisponivel.valor = 0;
    MulherDisponivel.valor = 0;

    pthread_t homem;
    pthread_t mulher;

    pthread_create(&homem, NULL, ChegarFestaHomem, NULL);
    pthread_create(&mulher, NULL, ChegarFestaMulher, NULL);

    sleep(1);

    pthread_join(homem, NULL);
    pthread_join(mulher, NULL);

    return 0;
}