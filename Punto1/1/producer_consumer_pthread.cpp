#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <time.h>

#define N 10       // tamaño del buffer
#define PRODUCERS 2
#define CONSUMERS 2

int buffer[N];   // buffer compartido
pthread_mutex_t mutex;
sem_t empty, full;

// función productor
void* producer(void* arg) {
    int id = *(int*)arg;
    while (1) {
        int item = rand() % 9 + 1; // valor aleatorio 1–9
        sem_wait(&empty);          // espera espacio libre
        pthread_mutex_lock(&mutex);

        // busca primer 0
        int placed = 0;
        for (int i = 0; i < N; i++) {
            if (buffer[i] == 0) {
                buffer[i] = item;
                printf("Producer %d produjo %d en pos %d\n", id, item, i);
                placed = 1;
                break;
            }
        }

        pthread_mutex_unlock(&mutex);
        sem_post(&full);

        if (!placed) break; // buffer lleno
        sleep(rand() % 3 + 1); // espera aleatoria
    }
    pthread_exit(NULL);
}

// función consumidor
void* consumer(void* arg) {
    int id = *(int*)arg;
    while (1) {
        sem_wait(&full);           // espera item
        pthread_mutex_lock(&mutex);

        int taken = 0;
        for (int i = 0; i < N; i++) {
            if (buffer[i] != 0) {
                int k = buffer[i];
                buffer[i] = 0;
                printf("Consumer %d consumió %d en pos %d\n", id, k, i);
                taken = 1;
                break;
            }
        }

        pthread_mutex_unlock(&mutex);
        sem_post(&empty);

        if (!taken) break; // buffer vacío
        sleep(rand() % 3 + 1); // espera según valor
    }
    pthread_exit(NULL);
}

int main() {
    srand(time(NULL));
    pthread_t prod[PRODUCERS], cons[CONSUMERS];
    int ids[PRODUCERS > CONSUMERS ? PRODUCERS : CONSUMERS];

    // inicializar buffer
    for (int i = 0; i < N; i++) buffer[i] = 0;

    pthread_mutex_init(&mutex, NULL);
    sem_init(&empty, 0, N);
    sem_init(&full, 0, 0);

    // crear productores
    for (int i = 0; i < PRODUCERS; i++) {
        ids[i] = i + 1;
        pthread_create(&prod[i], NULL, producer, &ids[i]);
    }

    // crear consumidores
    for (int i = 0; i < CONSUMERS; i++) {
        ids[i] = i + 1;
        pthread_create(&cons[i], NULL, consumer, &ids[i]);
    }

    // esperar hilos
    for (int i = 0; i < PRODUCERS; i++) pthread_join(prod[i], NULL);
    for (int i = 0; i < CONSUMERS; i++) pthread_join(cons[i], NULL);

    pthread_mutex_destroy(&mutex);
    sem_destroy(&empty);
    sem_destroy(&full);

    return 0;
}