#include <iostream>
#include <pthread.h>
#include <vector>
#include <cstdlib>

using namespace std;

const int m = 6; // filas
const int n = 4; // columnas
const int NUM_THREADS = 3;

vector<vector<double>> A(m, vector<double>(n));
vector<double> x(n), y(m);

struct ThreadData {
    int rank;
    int thread_count;
};

void* Pth_mat_vect(void* rank) {
    ThreadData* data = (ThreadData*) rank;
    int my_rank = data->rank;
    int thread_count = data->thread_count;

    int local_m = m / thread_count;
    int my_first_row = my_rank * local_m;
    int my_last_row  = (my_rank + 1) * local_m - 1;

    for (int i = my_first_row; i <= my_last_row; i++) {
        y[i] = 0.0;
        for (int j = 0; j < n; j++) {
            y[i] += A[i][j] * x[j];
        }
    }
    return NULL;
}

int main() {
    srand(time(NULL));

    // Generar matriz A y vector x aleatorios
    cout << "Matriz A:\n";
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < n; j++) {
            A[i][j] = rand() % 10;
            cout << A[i][j] << " ";
        }
        cout << endl;
    }

    cout << "\nVector x:\n";
    for (int j = 0; j < n; j++) {
        x[j] = rand() % 10;
        cout << x[j] << " ";
    }
    cout << endl << endl;

    pthread_t threads[NUM_THREADS];
    ThreadData thread_data[NUM_THREADS];

    // Crear hilos
    for (int t = 0; t < NUM_THREADS; t++) {
        thread_data[t] = {t, NUM_THREADS};
        pthread_create(&threads[t], NULL, Pth_mat_vect, (void*)&thread_data[t]);
    }

    // Esperar hilos
    for (int t = 0; t < NUM_THREADS; t++) {
        pthread_join(threads[t], NULL);
    }

    cout << "Resultado y = A * x:\n";
    for (int i = 0; i < m; i++) {
        cout << y[i] << " ";
    }
    cout << endl;

    return 0;
}
