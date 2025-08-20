#include <iostream>
#include <pthread.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>   // para medir tiempo

using namespace std;
using namespace std::chrono;

const int N = 100000000; // 100 millones
const int NUM_THREADS = 16;

vector<int> arr(N);
long long global_sum = 0;
pthread_mutex_t lock_sum;

struct ThreadData {
    int start, end;
};

void* partial_sum(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    long long local_sum = 0;

    // Obtener el ID del hilo
    pthread_t tid = pthread_self();
    cout << "Hilo creado con ID: " << tid 
         << " trabajando desde " << data->start 
         << " hasta " << data->end << endl;

    for (int i = data->start; i < data->end; i++) {
        local_sum += arr[i];
    }

    // Región crítica
    pthread_mutex_lock(&lock_sum);
    global_sum += local_sum;
    pthread_mutex_unlock(&lock_sum);

    pthread_exit(NULL);
}


int main() {
    srand(time(NULL));
    for (int i = 0; i < N; i++) arr[i] = rand() % 10; // llenar arreglo con valores 0–9

    pthread_t threads[NUM_THREADS];
    ThreadData thread_data[NUM_THREADS];

    auto start = high_resolution_clock::now();

    pthread_mutex_init(&lock_sum, NULL);

    int chunk = N / NUM_THREADS;
    for (int t = 0; t < NUM_THREADS; t++) {
        thread_data[t].start = t * chunk;
        thread_data[t].end = (t == NUM_THREADS - 1) ? N : (t + 1) * chunk;
        pthread_create(&threads[t], NULL, partial_sum, (void*)&thread_data[t]);
    }

    for (int t = 0; t < NUM_THREADS; t++) {
        pthread_join(threads[t], NULL);
    }

    pthread_mutex_destroy(&lock_sum);

    auto stop = high_resolution_clock::now();
    duration<double> elapsed = stop - start;

    cout << "Suma total (Pthreads) = " << global_sum << endl;
    cout << "Tiempo CPU: " << elapsed.count() << " segundos" << endl;
    return 0;
}
