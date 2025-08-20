#include <iostream>
#include <vector>
#include <cstdlib>
#include <omp.h>
#include <ctime>
using namespace std;

int main() {
    const int m = 6, n = 4;
    vector<vector<double>> A(m, vector<double>(n));
    vector<double> x(n), y(m);

    srand(time(NULL));

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

    #pragma omp parallel for
    for (int i = 0; i < m; i++) {
        y[i] = 0.0;
        for (int j = 0; j < n; j++) {
            y[i] += A[i][j] * x[j];
        }
    }

    cout << "Resultado y = A * x:\n";
    for (int i = 0; i < m; i++) {
        cout << y[i] << " ";
    }
    cout << endl;

    return 0;
}
