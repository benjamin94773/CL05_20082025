#include <omp.h>
#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace std;

int main() {
    const int N = 100000000;
    vector<int> arr(N, 1);
    long long sum = 0;

    srand(time(NULL));
    for (int i = 0; i < N; i++) arr[i] = rand() % 10;

    double start = omp_get_wtime();

    #pragma omp parallel
    {
        int tid = omp_get_thread_num();
        int total = omp_get_num_threads();

        // Solo un print por hilo para no saturar la salida
        #pragma omp critical
        cout << "Hilo " << tid << " de " << total << " hilos creados" << endl;

        // Ahora hacemos la suma en paralelo
        #pragma omp for reduction(+:sum)
        for (int i = 0; i < N; i++) {
            sum += arr[i];
        }
    }

    double stop = omp_get_wtime();

    cout << "Suma total (OpenMP) = " << sum << endl;
    cout << "Tiempo CPU: " << (stop - start) << " segundos" << endl;
    return 0;
}
