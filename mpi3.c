/**
 * Exemplo com MPI_ANY_SOURCE e MPI_ANY_TAG
 */
 
#include <stdio.h>
#include <string.h>  
#include <mpi.h>     

const int MAX_STRING = 100;

int main(void) {
   char       message[MAX_STRING];  
   int        comm_size;               
   int        my_rank;               

   MPI_Init(NULL, NULL); 
   MPI_Comm_size(MPI_COMM_WORLD, &comm_size); 
   MPI_Comm_rank(MPI_COMM_WORLD, &my_rank); 


   if (my_rank != 0) { 
      /* Criação da mensagem */
      sprintf(message, "Msg do processo %d of %d!", my_rank, comm_size); 
      
      /* Envio da mensagem para o processo 0 */
      MPI_Send(message, strlen(message)+1, MPI_CHAR, 0, 0, MPI_COMM_WORLD); 
   } else {  
      /* Impressão da mensagem do processo centralizador */
      printf("Processo centralizador em execução: %d of %d!\n", my_rank, comm_size);
      
      for (int q = 1; q < comm_size; q++) {
         /* Recepção da mensagem do processo q */
         MPI_Recv(message, MAX_STRING, MPI_CHAR, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
      
         /* Impressão da mensagem do processo q */
         printf("O processo %d recebeu a seguinte mensagem: %s\n", my_rank, message);
      } 
   }

   /* Finaliza MPI */
   MPI_Finalize(); 

   return 0;
}  /* main */
