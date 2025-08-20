#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <time.h>

double a, b; 
int n, thread_count;
double global_result = 0.0;
pthread_mutex_t mutex;

// Función a integrar
double f(double x) {
    return x * x; // ejemplo: f(x) = x^2
}

void* Trap(void* rank) {
    long my_rank = (long)rank;
    double h = (b - a) / n;
    int local_n = n / thread_count;

    double local_a = a + my_rank * local_n * h;
    double local_b = local_a + local_n * h;

    double my_result = (f(local_a) + f(local_b)) / 2.0;
    for (int i = 1; i < local_n; i++) {
        double x = local_a + i * h;
        my_result += f(x);
    }
    my_result *= h;

    pthread_mutex_lock(&mutex);
    global_result += my_result;
    pthread_mutex_unlock(&mutex);

    return NULL;
}

int main(int argc, char* argv[]) {
    if (argc != 5) {
        printf("Uso: %s <num_threads> <a> <b> <n>\n", argv[0]);
        return 1;
    }

    thread_count = strtol(argv[1], NULL, 10);
    a = atof(argv[2]);
    b = atof(argv[3]);
    n = strtol(argv[4], NULL, 10);

    pthread_t threads[thread_count];
    pthread_mutex_init(&mutex, NULL);

    clock_t start = clock();

    for (long thread = 0; thread < thread_count; thread++) {
        pthread_create(&threads[thread], NULL, Trap, (void*)thread);
    }

    for (long thread = 0; thread < thread_count; thread++) {
        pthread_join(threads[thread], NULL);
    }

    clock_t end = clock();
    double time_taken = ((double)(end - start)) / CLOCKS_PER_SEC;

    printf("Integral de %.2f a %.2f = %.15f\n", a, b, global_result);
    printf("Tiempo (Pthreads, %d hilos): %f segundos\n", thread_count, time_taken);

    pthread_mutex_destroy(&mutex);
    return 0;
}
