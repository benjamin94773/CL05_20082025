// matrix_mul_pthreads.cpp
#include <iostream>
#include <pthread.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>   // para medir tiempo

using namespace std;
using namespace std::chrono;

const int N = 10;  // filas de A
const int M = 10;  // columnas de A y filas de B
const int P = 10;  // columnas de B
const int NUM_THREADS = 8;

vector<vector<int>> A(N, vector<int>(M));
vector<vector<int>> B(M, vector<int>(P));
vector<vector<int>> C(N, vector<int>(P));

struct ThreadData {
    int start_row, end_row;
};

void* multiply(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    for (int i = data->start_row; i < data->end_row; i++) {
        for (int j = 0; j < P; j++) {
            int sum = 0;
            for (int k = 0; k < M; k++) {
                sum += A[i][k] * B[k][j];
            }
            C[i][j] = sum;
        }
    }
    pthread_exit(NULL);
}

int main() {
    srand(time(NULL));

    // Generar matrices aleatorias
    for (int i = 0; i < N; i++)
        for (int j = 0; j < M; j++)
            A[i][j] = rand() % 10;

    for (int i = 0; i < M; i++)
        for (int j = 0; j < P; j++)
            B[i][j] = rand() % 10;

    pthread_t threads[NUM_THREADS];
    ThreadData thread_data[NUM_THREADS];

    auto start = high_resolution_clock::now();

    int chunk = N / NUM_THREADS;
    for (int t = 0; t < NUM_THREADS; t++) {
        thread_data[t].start_row = t * chunk;
        thread_data[t].end_row = (t == NUM_THREADS - 1) ? N : (t + 1) * chunk;
        pthread_create(&threads[t], NULL, multiply, (void*)&thread_data[t]);
    }

    for (int t = 0; t < NUM_THREADS; t++) {
        pthread_join(threads[t], NULL);
    }

    auto stop = high_resolution_clock::now();
    duration<double> elapsed = stop - start;

    cout << "Tiempo CPU: " << elapsed.count() << " segundos" << endl;
    cout << "Multiplicación completada (Pthreads)" << endl;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < P; j++) {
            cout << C[i][j] << " ";
        }
        cout << endl;
    }
    return 0;
}
