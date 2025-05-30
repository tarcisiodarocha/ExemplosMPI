#include <stdio.h>
#include <string.h>  
#include <math.h>
#include <mpi.h>     

const int MAX_STRING = 100;

int main(void) {
   int       valor;  
   int       x;
   int       comm_size;               
   int       my_rank;               

   MPI_Init(NULL, NULL); 
   MPI_Comm_size(MPI_COMM_WORLD, &comm_size); 
   MPI_Comm_rank(MPI_COMM_WORLD, &my_rank); 
   
   if (my_rank == 0){
      x = 3;
   }   
   
   MPI_Bcast(&x, 1, MPI_INT, 0, MPI_COMM_WORLD);

   if (my_rank == 0) { 
      int total = 0; 
      for (int q = 1; q < comm_size; q++) {
         /* Recepção da mensagem do processo q */
         MPI_Recv(&valor, 1, MPI_INT, q, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
         total = total + valor;
      } 
      /* Impressão o resultado final */
      printf("Resultado final do processo %d: %d\n", my_rank, total);
      
   } else if (my_rank == 1) {  
      valor = x + x;
       /* Envio da mensagem para o processo 0 */
      MPI_Send(&valor, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
   } else if (my_rank == 2) {  
      valor = x * x;
       /* Envio da mensagem para o processo 0 */
      MPI_Send(&valor, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
   } else if (my_rank == 3) {  
      valor = pow(x, x);
       /* Envio da mensagem para o processo 0 */
      MPI_Send(&valor, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
   } else {  
      valor = x;
       /* Envio da mensagem para o processo 0 */
      MPI_Send(&valor, 1, MPI_INT, 0, 0, MPI_COMM_WORLD);
   }
   /* Finaliza MPI */
   MPI_Finalize(); 

   return 0;
}  /* main */
