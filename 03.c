#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <stdint.h>
#include "semaforo.h"
#include "fifo.h"

pthread_t *homens;
pthread_t *mulheres;

Semaforo MulherDisponivel;
Semaforo HomemDisponivel;

next_link idsM, idsH;
int idH = 1, idM = 1;
int running = 1;

void *ChegarFestaHomem(void *arg)
{
    while (running)
    {
        int idHAtual = idH;
        idH++;
        printf("\nChegou um homem id: %d!", idHAtual);
        idsH = fifo_add(&idsH, idHAtual);
        semaforo_signal(&HomemDisponivel);
        semaforo_wait(&MulherDisponivel);
        next_link vitrineM = fifo_remove(&idsM);
        printf("\nFoi feito um casal! Com: M: %d e H: %d", vitrineM->item, idHAtual);
    }
    return NULL;
}

void *ChegarFestaMulher(void *arg)
{
    while (running)
    {
        int idMAtual = idM;
        idM++;
        printf("\nChegou uma mulher id: %d!", idMAtual);
        idsM = fifo_add(&idsM, idMAtual);
        semaforo_signal(&MulherDisponivel);
        semaforo_wait(&HomemDisponivel);
        next_link vitrineH = fifo_remove(&idsH);
        printf("\nFoi feito um casal! Com: M: %d e H: %d", idMAtual, vitrineH->item);
    }
    return NULL;
}

void ChegarAlguem()
{
    int i = 0;
    int h = 0;
    int m = 0;
    while (i < 50)
    {
        if (rand() % 2)
        {
            pthread_create(&mulheres[m], NULL, ChegarFestaMulher, NULL);
            m++;
        }
        else
        {
            pthread_create(&homens[h], NULL, ChegarFestaHomem, NULL);
            h++;
        }
        i++;
    }
}

void Sincronizar()
{
    int i = 0;
    for (int i = 0; i < idH - 1; i++)
    {
        pthread_join(homens[i], NULL);
    }

    for (int i = 0; i < idM - 1; i++)
    {
        pthread_join(mulheres[i], NULL);
    }
}

int main(void)
{
    srand(time(NULL));

    semaforo_init(&HomemDisponivel, 0);
    semaforo_init(&MulherDisponivel, 0);

    homens = (pthread_t *)malloc(sizeof(pthread_t) * 40);
    mulheres = (pthread_t *)malloc(sizeof(pthread_t) * 40);

    idsH = NULL;
    idsM = NULL;

    printf("ESTOU VIVO");

    ChegarAlguem();

    sleep(0.5f);
    running = 0;

    Sincronizar();

    free(homens);
    free(mulheres);
    return 0;
}