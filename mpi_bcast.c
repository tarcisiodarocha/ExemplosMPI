#include <stdio.h>
#include <mpi.h>

int main(int argc, char* argv[]) {
    int my_rank, comm_size;
    int numero;

    MPI_Init(&argc, &argv);                  // Inicializa MPI
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank); // Rank do processo atual
    MPI_Comm_size(MPI_COMM_WORLD, &comm_size); // Número total de processos

    if (my_rank == 0) {
        // Apenas o processo 0 define o valor
        numero = 42;
        printf("Processo %d definiu o número %d para ser enviado aos demais.\n", my_rank, numero);
    }

    // Broadcast: todos os processos recebem o valor de "numero" vindo do processo 0
    MPI_Bcast(&numero, 1, MPI_INT, 0, MPI_COMM_WORLD);

    // Cada processo imprime o valor recebido
    printf("Processo %d recebeu o número: %d\n", my_rank, numero);

    MPI_Finalize(); // Finaliza MPI
    return 0;
}
