# CL05_20082025
## Instrucciones de compilación y ejecución

### Productor-Consumidor
```bash
g++ -o prodcons_pthreads producer_consumer_pthread.cpp -lpthread
./prodcons_pthreads

g++ -o prodcons_omp producer_consumer_omp.cpp -fopenmp
./prodcons_omp
```
### Multiplicación Matriz-Vector
```bash

g++ -o matvec_pthreads matrix_vector_pth.cpp -lpthread
./matvec_pthreads

g++ -o matvec_omp matrix_vector_omp.cpp -fopenmp
./matvec_omp

```
### Regla del Trapecio
```bash
g++ -fopenmp trap_omp.cpp -o trap_omp -lm
./trap_omp 4 0 1 1000000

g++ trap_pthreads.cpp -o trap_pthreads -lpthread -lm
./trap_pthreads 4 0 1 1000000
```

### Count Sort
```bash
g++ count_sort_omp.cpp -fopenmp -o count_sort_omp
./count_sort_omp 4 10000

g++ count_sort_pthreads.cpp -o count_sort_pthreads -lpthread
./count_sort_pthreads 4 10000
```

### Multiplicación de Matrices
```bash
g++ matrix_mul_pthreads.cpp -o mat_pth -pthread
./mat_pth

g++ matrix_mul_openmp.cpp -o mat_omp -fopenmp
./mat_omp
```
### Suma de un Arreglo Grande
```bash
g++ ejercicio2omp.cpp -o ejerc2 -fopenmp
./ejerc2

g++ ejercicio2pthread.cpp -o suma_pthreads -pthread
./suma_pthreads
```

