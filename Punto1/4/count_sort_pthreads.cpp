#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>

int* a;
int* temp;
int n, thread_count;

void* thread_count_sort(void* rank) {
    long my_rank = (long) rank;
    int local_n = n / thread_count;
    int my_first = my_rank * local_n;
    int my_last = (my_rank == thread_count - 1) ? n : (my_rank + 1) * local_n;

    for (int i = my_first; i < my_last; i++) {
        int count = 0;
        for (int j = 0; j < n; j++) {
            if (a[j] < a[i]) 
                count++;
            else if (a[j] == a[i] && j < i)
                count++;
        }
        temp[count] = a[i];
    }
    return NULL;
}

void Count_sort_pthreads(int thread_count) {
    pthread_t* threads = (pthread_t*) malloc(thread_count * sizeof(pthread_t));

    for (long t = 0; t < thread_count; t++) {
        pthread_create(&threads[t], NULL, thread_count_sort, (void*) t);
    }
    for (long t = 0; t < thread_count; t++) {
        pthread_join(threads[t], NULL);
    }

    memcpy(a, temp, n * sizeof(int));
    free(threads);
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Uso: %s <num_threads> <n>\n", argv[0]);
        return 1;
    }

    thread_count = strtol(argv[1], NULL, 10);
    n = strtol(argv[2], NULL, 10);

    a = (int*) malloc(n * sizeof(int));
    temp = (int*) malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) a[i] = rand() % 100;

    clock_t start = clock();
    Count_sort_pthreads(thread_count);
    clock_t end = clock();

    double elapsed = ((double)(end - start)) / CLOCKS_PER_SEC;
    printf("Tiempo (Pthreads con %d hilos): %f segundos\n", thread_count, elapsed);

    free(a);
    return 0;
}
