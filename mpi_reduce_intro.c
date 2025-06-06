#include <mpi.h>
#include <stdio.h>

int main(int argc, char** argv) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int local_value = rank + 1;  // Cada processo tem um valor diferente (1, 2, 3, ...)
    int global_sum = local_value; // Inicializa com o valor local

    if (rank == 0) {
        // Processo raiz (0) recebe dados de todos os outros processos
        int received_value;
        for (int i = 1; i < size; i++) {
            MPI_Recv(&received_value, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            global_sum += received_value;
        }
        printf("Soma global = %d\n", global_sum);
    } else {
        // Outros processos enviam seus valores para o processo raiz
        MPI_Send(&local_value, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
    }

    MPI_Finalize();
    return 0;
}