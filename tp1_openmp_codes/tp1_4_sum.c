#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#define HYPERTHREADING 1      // 1 if hyperthreading is on, 0 otherwise
#define ERROR          1.e-20 // Acceptable precision
#define MAX_VAL        5      // Random values are [0, MAX_VAL]

// Matrix and vector sizes (5120: UHD TV)
#define N 5120000

// Reference computation kernel (do not touch)
void sum_reference(double* psum, double a[N]) {
  double sum = 0.;

  for (size_t i = 0; i < N; i++) {
    sum += a[i];
  }
  *psum = sum;
}

// Computation kernel (to parallelize)
void sum_kernel(double* psum, double a[N]) {
  double sum = 0.;

  for (size_t i = 0; i < N; i++) {
    sum += a[i];
  }
  *psum = sum;
}

int main() {
  double* a = malloc(N * sizeof(double));
  double ref, sum;
  double time_reference, time_kernel, speedup, efficiency;

  // Initialization by random values
  srand((unsigned int)time(NULL));
  for (size_t i = 0; i < N; i++) {
    a[i] = (float)rand()/(float)(RAND_MAX/MAX_VAL);
  }

  time_reference = omp_get_wtime();
  sum_reference(&ref, a);
  time_reference = omp_get_wtime() - time_reference;
  printf("Reference time : %3.5lf s\n", time_reference);

  time_kernel = omp_get_wtime();
  sum_kernel(&sum, a);
  time_kernel = omp_get_wtime() - time_kernel;
  printf("Kernel time    : %3.5lf s\n", time_kernel);

  speedup = time_reference / time_kernel;
  efficiency = speedup / (omp_get_num_procs() / (1 + HYPERTHREADING));
  printf("Speedup ------ : %3.5lf\n", speedup);
  printf("Efficiency --- : %3.5lf\n", efficiency);

  // Check if the result differs from the reference
  if (abs(ref - sum) > ERROR) {
    printf("Bad results :-(((\n");
    exit(1);
  }
  printf("OK results :-)\n");

  free(a);
  return 0;
}
