// matrix_mul_openmp.cpp
#include <iostream>
#include <omp.h>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <chrono>   // para medir tiempo

using namespace std;
using namespace std::chrono;

const int N = 10;  // filas de A
const int M = 10;  // columnas de A y filas de B
const int P = 10;  // columnas de B

vector<vector<int>> A(N, vector<int>(M));
vector<vector<int>> B(M, vector<int>(P));
vector<vector<int>> C(N, vector<int>(P));

int main() {
    srand(time(NULL));

    // Generar matrices aleatorias
    for (int i = 0; i < N; i++)
        for (int j = 0; j < M; j++)
            A[i][j] = rand() % 10;

    for (int i = 0; i < M; i++)
        for (int j = 0; j < P; j++)
            B[i][j] = rand() % 10;

    double start = omp_get_wtime();

    #pragma omp parallel for
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < P; j++) {
            int sum = 0;
            for (int k = 0; k < M; k++) {
                sum += A[i][k] * B[k][j];
            }
            C[i][j] = sum;
        }
    }

    double stop = omp_get_wtime();

    cout << "Tiempo CPU: " << (stop - start) << " segundos" << endl;

    cout << "Multiplicación completada (OpenMP)" << endl;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < P; j++) {
            cout << C[i][j] << " ";
        }
        cout << endl;
    }
    return 0;
}
