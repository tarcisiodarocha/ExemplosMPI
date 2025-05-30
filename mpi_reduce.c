#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int local_value = rank + 1;  // Cada processo tem um valor diferente (1, 2, 3, ...)
    int global_sum;

    // Soma todos os 'local_value' no processo 0
    MPI_Reduce(&local_value, &global_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        printf("Soma global = %d\n", global_sum);  // Ex: com 4 processos, imprime 10 (1+2+3+4)
    }

    MPI_Finalize();
    return 0;
}