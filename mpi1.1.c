#include <stdio.h>
#include <string.h>  
#include <mpi.h>     

int main(void) {
   int        comm_sz;               /* Número de processos    */
   int        my_rank;               /* Rank   do processo     */

   /* Inicia MPI */
   MPI_Init(NULL, NULL); 

   /* Obter número de processos */
   MPI_Comm_size(MPI_COMM_WORLD, &comm_sz); 

   /* Obter rank (id) do processo */
   MPI_Comm_rank(MPI_COMM_WORLD, &my_rank); 
   char *nome; 
   if (my_rank == 0){
      nome = "Joao";
   } else if (my_rank == 1){
      nome = "Maria";
   } else if (my_rank == 2){
      nome = "Pedro";
   } else if (my_rank == 3){
      nome = "Marta";
   } else{
      nome = "Outro";
   }

   printf("Rank: %d Nome: %s \n", my_rank, nome);
 
   /* Encerra MPI */
   MPI_Finalize(); 

   return 0;
}  /* main */
