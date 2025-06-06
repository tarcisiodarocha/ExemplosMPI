/* File:     mpi_trap1.c
 * Purpose:  Use MPI to implement a parallel version of the trapezoidal 
 *           rule.  In this version the endpoints of the interval and
 *           the number of trapezoids are hardwired.
 *
 * Input:    None.
 * Output:   Estimate of the integral from a to b of f(x)
 *           using the trapezoidal rule and n trapezoids.
 *
 * Compile:  mpicc -g -Wall -o mpi_trap1 mpi_trap1.c
 * Run:      mpiexec -n <number of processes> ./mpi_trap1
 *
 * Algorithm:
 *    1.  Each process calculates "its" interval of
 *        integration.
 *    2.  Each process estimates the integral of f(x)
 *        over its interval using the trapezoidal rule.
 *    3a. Each process != 0 sends its integral to 0.
 *    3b. Process 0 sums the calculations received from
 *        the individual processes and prints the result.
 *
 * Note:  f(x), a, b, and n are all hardwired.
 *
 * IPP:   Section 3.2.2 (pp. 96 and ff.)
 * 
 * Plotar função online: https://www.desmos.com/calculator?lang=pt-BR
 *     f(x)=x^2-4x +8
 *     a = 0
 *     b = 3
 *     0 <= y <= f(x){(a-x)(b-x)<0}
 */
#include <stdio.h>
#include <mpi.h>
#include <math.h>

/* Calculate local integral  */
double Trap(double left_endpt, double right_endpt, int trap_count, 
   double base_len);    

/* Function we're integrating */
double f(double x); 

int main(void) {
   int my_rank, comm_sz, n = 2000000000, local_n;   
   double a = 0.0, b = 10.0, h, local_a, local_b;
   double local_int, total_int;
   int source;
   double start_time, end_time;  // Variáveis para medição de tempo

   /* Inicializa MPI e marca o tempo de início */
   MPI_Init(NULL, NULL);
   start_time = MPI_Wtime();

   /* Get my process rank */
   MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

   /* Find out how many processes are being used */
   MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

   h = (b-a)/n;          /* h is the same for all processes */
   local_n = n/comm_sz;  /* So is the number of trapezoids  */

   /* Length of each process' interval of
    * integration = local_n*h.  So my interval
    * starts at: */
   local_a = a + my_rank*local_n*h;
   local_b = local_a + local_n*h;
   local_int = Trap(local_a, local_b, local_n, h);

   /* Add up the integrals calculated by each process */
   if (my_rank != 0) { 
      MPI_Send(&local_int, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD); 
   } else {
      total_int = local_int;
      for (source = 1; source < comm_sz; source++) {
         MPI_Recv(&local_int, 1, MPI_DOUBLE, source, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
         total_int += local_int;
      }
   } 

   /* Marca o tempo final */
   end_time = MPI_Wtime();

   /* Print the result */
   if (my_rank == 0) {
      printf("With n = %d trapezoids, our estimate\n", n);
      printf("of the integral from %f to %f = %.15e\n",
          a, b, total_int);
      printf("Elapsed time = %.6f seconds\n", end_time - start_time);
   }

   /* Shut down MPI */
   MPI_Finalize();

   return 0;
} /*  main  */


/*------------------------------------------------------------------
 * Function:     Trap
 * Purpose:      Serial function for estimating a definite integral 
 *               using the trapezoidal rule
 * Input args:   left_endpt
 *               right_endpt
 *               trap_count 
 *               base_len
 * Return val:   Trapezoidal rule estimate of integral from
 *               left_endpt to right_endpt using trap_count
 *               trapezoids
 */
double Trap(
      double left_endpt  /* in */, 
      double right_endpt /* in */, 
      int    trap_count  /* in */, 
      double base_len    /* in */) {
   
   double estimate = 0.0, x;  // Inicialize estimate com 0.0
   int i;

   for (i = 0; i <= trap_count-1; i++) {
      x = left_endpt + i*base_len;
      estimate += (f(x) + f(x+base_len))/2.0;
   }
   estimate = estimate*base_len;

   return estimate;
} /*  Trap  */


/*------------------------------------------------------------------
 * Function:    f
 * Purpose:     Compute value of function to be integrated
 * Input args:  x
 */
double f(double x) {
   //return x*x - 4*x + 8;
   return exp(sin(x)*cos(x)) * log(x+1) * sqrt(x*x*x + x*x + 1);
   //return x*x;
} /* f */