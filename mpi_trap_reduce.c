/* File:     mpi_trap_reduce.c
 * Purpose:  Use MPI to implement a parallel version of the trapezoidal 
 *           rule using MPI_Reduce. In this version the endpoints of the interval and
 *           the number of trapezoids are hardwired.
 *
 * Input:    None.
 * Output:   Estimate of the integral from a to b of f(x)
 *           using the trapezoidal rule and n trapezoids.
 *
 * Compile:  mpicc -g -Wall -o mpi_trap_reduce mpi_trap_reduce.c -lm
 * Run:      mpiexec -n <number of processes> ./mpi_trap_reduce
 *
 * Algorithm:
 *    1.  Each process calculates "its" interval of integration.
 *    2.  Each process estimates the integral of f(x) over its interval.
 *    3.  All processes use MPI_Reduce to sum their partial results.
 *
 * Note:  f(x), a, b, and n are all hardwired.
 */
#include <stdio.h>
#include <mpi.h>
#include <math.h>

/* Calculate local integral */
double Trap(double local_a, double local_b, int local_n, double h);    

/* Function we're integrating */
double f(double x); 

int main(void) {
   int my_rank, comm_sz, n = 1000000000, local_n;   
   double a = 1.0, b = 5.0, h, local_a, local_b;
   double local_int, total_int;
   double start_time, end_time;

   MPI_Init(NULL, NULL);
   start_time = MPI_Wtime();

   MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
   MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

   h = (b-a)/n;
   local_n = n/comm_sz;

   local_a = a + my_rank*local_n*h;
   local_b = local_a + local_n*h;
   local_int = Trap(local_a, local_b, local_n, h);

   MPI_Reduce(&local_int, &total_int, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

   end_time = MPI_Wtime();

   if (my_rank == 0) {
      printf("With n = %d trapezoids, our estimate\n", n);
      printf("of the integral from %f to %f = %.15e\n", a, b, total_int);
      printf("Elapsed time = %.6f seconds\n", end_time - start_time);
   }

   MPI_Finalize();
   return 0;
}

/*------------------------------------------------------------------
 * Function:     Trap
 * Purpose:      Serial function for estimating a definite integral 
 *               using the trapezoidal rule
 */
double Trap(double local_a, double local_b, int local_n, double h) {
   double estimate = 0.0, x;
   int i;

   for (i = 0; i <= local_n-1; i++) {
      x = local_a + i*h;
      estimate += (f(x) + f(x+h))/2.0;
   }
   estimate = estimate*h;

   return estimate;
}

/*------------------------------------------------------------------
 * Function:    f
 * Purpose:     Compute value of function to be integrated
 */
double f(double x) {
  return exp(sin(x)*cos(x)) * log(x+1) * sqrt(x*x*x + x*x + 1);
  // return x*x - 4*x + 8;
   
}