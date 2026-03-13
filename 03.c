#include <pthread.h>
#include <semaphore.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

pthread_t *homens;
pthread_t *mulheres;

sem_t sem_mulher;
sem_t sem_homem;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

int id_mulher_atual = -1;
int id_homem_atual = -1;

void *ChegarHomem(void *arg) {
    int id = (int)(intptr_t)arg;
    printf("\nChegou um homem! %d", id);

    pthread_mutex_lock(&mutex);

    id_homem_atual = id;
    sem_post(&sem_homem);
    sem_wait(&sem_mulher);

    printf("\nHomem %d formou casal com Mulher %d!", id, id_mulher_atual);

    pthread_mutex_unlock(&mutex);
    return NULL;
}

void *ChegarMulher(void *arg) {
    int id = (int)(intptr_t)arg;
    printf("\nChegou uma mulher! %d", id);

    sem_wait(&sem_homem);
    id_mulher_atual = id;
    int id_h = id_homem_atual;
    sem_post(&sem_mulher);

    return NULL;
}

void ChegarAlguem() {
    int i = 0;
    int h = 0;
    int m = 0;
    while (i < 50) {
        if (rand() % 2) {
            pthread_create(&mulheres[m], NULL, ChegarMulher, (void *)(intptr_t)(m + 1));
            m++;
        } else {
            pthread_create(&homens[h], NULL, ChegarHomem, (void *)(intptr_t)(h + 1));
            h++;
        }
        i++;
    }
}

void Sincronizar() {
    for (int i = 0; i < 50; i++) {
        if (homens[i] == 0)
            continue;
        pthread_join(homens[i], NULL);
    }

    for (int i = 0; i < 50; i++) {
        if (mulheres[i] == 0)
            continue;
        pthread_join(mulheres[i], NULL);
    }
}

int main(void) {
    srand(time(NULL));

    sem_init(&sem_mulher, 0, 0);
    sem_init(&sem_homem, 0, 0);

    homens = (pthread_t *)calloc(sizeof(pthread_t) * 50, 1);
    mulheres = (pthread_t *)calloc(sizeof(pthread_t) * 50, 1);

    ChegarAlguem();

    Sincronizar();

    free(homens);
    free(mulheres);
    return 0;
}
