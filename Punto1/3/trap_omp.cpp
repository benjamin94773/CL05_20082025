#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <math.h>

// Función a integrar
double f(double x) {
    return x * x; // ejemplo: f(x) = x^2
}

double Trap(double a, double b, int n, int thread_count) {
    double h = (b - a) / n;
    double global_result = (f(a) + f(b)) / 2.0;
    
    #pragma omp parallel for num_threads(thread_count) reduction(+:global_result)
    for (int i = 1; i < n; i++) {
        double x = a + i * h;
        global_result += f(x);
    }

    global_result *= h;
    return global_result;
}

int main(int argc, char* argv[]) {
    if (argc != 5) {
        printf("Uso: %s <num_threads> <a> <b> <n>\n", argv[0]);
        return 1;
    }

    int thread_count = strtol(argv[1], NULL, 10);
    double a = atof(argv[2]);
    double b = atof(argv[3]);
    int n = strtol(argv[4], NULL, 10);

    double start = omp_get_wtime();
    double result = Trap(a, b, n, thread_count);
    double end = omp_get_wtime();

    printf("Integral de %.2f a %.2f = %.15f\n", a, b, result);
    printf("Tiempo (OpenMP, %d hilos): %f segundos\n", thread_count, end - start);

    return 0;
}
