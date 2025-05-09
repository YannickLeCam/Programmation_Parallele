#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#define HYPERTHREADING 1 // 1 if hyperthreading is on, 0 otherwise
#define ERROR 1.e-20     // Acceptable precision
#define MAX_VAL 5        // Random values are [0, MAX_VAL]

// Matrix and vector sizes (5120: UHD TV)
#define N 5120

// Reference computation kernel 
void matvec_reference(double c[N], double A[N][N], double b[N])
{
  size_t i, j;

  for (i = 0; i < N; i++)
  {
    c[i] = 0.;
    for (j = 0; j < N; j++)
    {
      c[i] += A[i][j] * b[j];
    }
  }
}

// Computation kernel 
void matvec_kernel(double c[N], double A[N][N], double b[N])
{
  size_t i, j;
  #pragma omp parallel private(i,j) shared(A,b) num_threads(10)
  {
    #pragma omp for schedule(static)
    for (i = 0; i < N; i++)
    {
      c[i] = 0.;
      for (j = 0; j < N; j++)
      {
        c[i] += A[i][j] * b[j];
      }
    }
  }
}

int main()
{
  double *A = malloc(N * N * sizeof(double));
  double *b = malloc(N * sizeof(double));
  double *c = malloc(N * sizeof(double));
  double *ref = malloc(N * sizeof(double));
  double time_reference, time_kernel, speedup, efficiency;

  // Initialization by random values
  srand((unsigned int)time(NULL));
  for (size_t i = 0; i < N; i++)
    b[i] = (float)rand() / (float)(RAND_MAX / MAX_VAL);
  for (size_t i = 0; i < N * N; i++)
    A[i] = (float)rand() / (float)(RAND_MAX / MAX_VAL);

  time_reference = omp_get_wtime();
  matvec_reference(ref, (double(*)[N])A, b);
  time_reference = omp_get_wtime() - time_reference;
  printf("Reference time : %3.5lf s\n", time_reference);

  time_kernel = omp_get_wtime();
  matvec_kernel(c, (double(*)[N])A, b);
  time_kernel = omp_get_wtime() - time_kernel;
  printf("Kernel time -- : %3.5lf s\n", time_kernel);

  speedup = time_reference / time_kernel;
  efficiency = speedup / (omp_get_num_procs() / (1 + HYPERTHREADING));
  printf("Speedup ------ : %3.5lf\n", speedup);
  printf("Efficiency --- : %3.5lf\n", efficiency);

  // Check if the result differs from the reference
  for (size_t i = 0; i < N; i++)
  {
    // if (abs(ref[i] - c[i]) > ERROR) {
    if (ref[i] != c[i])
    {
      printf("Bad results :-(((\n");
      exit(1);
    }
  }
  printf("OK results :-)\n");

  free(A);
  free(b);
  free(c);
  free(ref);
  return 0;
}
