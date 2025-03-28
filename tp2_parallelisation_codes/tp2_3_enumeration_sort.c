#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <omp.h>
#define HYPERTHREADING 1 // 1 if hyperthreading is on, 0 otherwise
#define ERROR 1.e-20     // Acceptable precision
#define MAX_VAL 5        // Random values are [0, MAX_VAL]

// Matrix and vector sizes (5120: UHD TV)
#define N 10240

// Reference computation kernel 
void enumeration_sort_reference(double tab[N])
{
  size_t i, j;
  size_t *position = malloc(N * sizeof(size_t));
  double *copy = malloc(N * sizeof(size_t));

  for (i = 0; i < N; i++)
  {
    position[i] = 0;
    copy[i] = tab[i];
  }

  for (j = 0; j < N; j++)
  {
    for (i = 0; i < N; i++)
    {
      if ((tab[j] < tab[i]) || ((tab[i] == tab[j]) && (i < j)))
      {
        position[i]++;
      }
    }
  }

  for (i = 0; i < N; i++)
    tab[position[i]] = copy[i];

  free(position);
  free(copy);
}

// Computation kernel 
void enumeration_sort_kernel(double tab[N])
{
  size_t i, j;
  size_t *position = malloc(N * sizeof(size_t));
  double *copy = malloc(N * sizeof(size_t));

#pragma omp parallel private(i, j)
  {
#pragma omp for
    for (i = 0; i < N; i++)
    {
      position[i] = 0;
      copy[i] = tab[i];
    }
#pragma omp barrier
#pragma omp for schedule(static)
    for (j = 0; j < N; j++)
    {
      for (i = 0; i < N; i++)
      {
        if ((tab[j] < tab[i]) || ((tab[i] == tab[j]) && (i < j)))
        {
#pragma omp atomic update
          position[i]++;
        }
      }
    }
#pragma omp barrier
#pragma omp for
    for (i = 0; i < N; i++)
      tab[position[i]] = copy[i];
  }
  free(position);
  free(copy);
}

void print_sample(double tab[], size_t size, size_t sample_length)
{
  if (size <= 2 * sample_length)
  {
    for (size_t i = 0; i < size; i++)
      printf("%g ", tab[i]);
  }
  else
  {
    for (size_t i = 0; (i < size) && (i < sample_length); i++)
      printf("%g ", tab[i]);
    printf("... ");
    for (size_t i = size - sample_length; i < size; i++)
      printf("%g ", tab[i]);
  }
  printf("\n");
}

int main()
{
  double *a = malloc(N * sizeof(double));
  double *ref = malloc(N * sizeof(double));
  double time_reference, time_kernel, speedup, efficiency;

  // Initialization by random values
  srand((unsigned int)time(NULL));
  for (size_t i = 0; i < N; i++)
  {
    a[i] = (float)rand() / (float)(RAND_MAX / MAX_VAL);
    ref[i] = a[i];
  }

  time_reference = omp_get_wtime();
  enumeration_sort_reference(ref);
  time_reference = omp_get_wtime() - time_reference;
  printf("Reference time : %3.5lf s\n", time_reference);

  time_kernel = omp_get_wtime();
  enumeration_sort_kernel(a);
  time_kernel = omp_get_wtime() - time_kernel;
  printf("Kernel time -- : %3.5lf s\n", time_kernel);

  speedup = time_reference / time_kernel;
  efficiency = speedup / (omp_get_num_procs() / (1 + HYPERTHREADING));
  printf("Speedup ------ : %3.5lf\n", speedup);
  printf("Efficiency --- : %3.5lf\n", efficiency);

  print_sample(ref, N, 5);
  print_sample(a, N, 5);

  // Check if the result differs from the reference
  for (size_t i = 0; i < N; i++)
  {
    if (ref[i] != a[i])
    {
      printf("Bad results :-(((\n");
      exit(1);
    }
  }
  printf("OK results :-)\n");

  free(a);
  free(ref);
  return 0;
}
