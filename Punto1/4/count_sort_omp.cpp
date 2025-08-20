#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <omp.h>

void Count_sort(int a[], int n, int thread_count) {
    int* temp = (int*) malloc(n * sizeof(int));

    #pragma omp parallel for num_threads(thread_count)
    for (int i = 0; i < n; i++) {
        int count = 0;
        for (int j = 0; j < n; j++) {
            if (a[j] < a[i]) 
                count++;
            else if (a[j] == a[i] && j < i)
                count++;
        }
        temp[count] = a[i];
    }

    memcpy(a, temp, n * sizeof(int));
    free(temp);
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Uso: %s <num_threads> <n>\n", argv[0]);
        return 1;
    }

    int thread_count = strtol(argv[1], NULL, 10);
    int n = strtol(argv[2], NULL, 10);

    int* arr = (int*) malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) arr[i] = rand() % 100;

    double start = omp_get_wtime();
    Count_sort(arr, n, thread_count);
    double end = omp_get_wtime();

    printf("Tiempo (OpenMP con %d hilos): %f segundos\n", thread_count, end - start);

    free(arr);
    return 0;
}
