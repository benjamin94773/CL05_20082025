#include <iostream>
#include <omp.h>
#include <cstdlib>
#include <ctime>
#include <unistd.h>
using namespace std;

#define N 10        // tamaño del buffer
#define PRODUCERS 2
#define CONSUMERS 2

int buffer[N];      // buffer compartido
int count_items = 0; // cuántos elementos hay en el buffer

void producer(int id) {
    while (true) {
        int item = rand() % 9 + 1; // producir número 1–9

        // espera activa mientras el buffer esté lleno
        while (count_items == N) {
            #pragma omp flush(count_items)
        }

        #pragma omp critical
        {
            for (int i = 0; i < N; i++) {
                if (buffer[i] == 0) {
                    buffer[i] = item;
                    count_items++;
                    cout << "Producer " << id << " produjo " << item << " en pos " << i << endl;
                    break;
                }
            }
        }

        sleep(rand() % 3 + 1); // simula espera
    }
}

void consumer(int id) {
    while (true) {
        // espera activa mientras el buffer esté vacío
        while (count_items == 0) {
            #pragma omp flush(count_items)
        }

        #pragma omp critical
        {
            for (int i = 0; i < N; i++) {
                if (buffer[i] != 0) {
                    int k = buffer[i];
                    buffer[i] = 0;
                    count_items--;
                    cout << "Consumer " << id << " consumió " << k << " en pos " << i << endl;
                    break;
                }
            }
        }

        sleep(rand() % 3 + 1); // simula espera
    }
}

int main() {
    srand(time(NULL));

    // inicializar buffer vacío
    for (int i = 0; i < N; i++) buffer[i] = 0;

    #pragma omp parallel num_threads(PRODUCERS + CONSUMERS)
    {
        int tid = omp_get_thread_num();
        if (tid < PRODUCERS) {
            producer(tid + 1);
        } else {
            consumer(tid + 1 - PRODUCERS);
        }
    }

    return 0;
}
