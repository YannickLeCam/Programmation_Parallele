#include <stdio.h>

void nb_thread_kernel() {
  size_t nb_threads = 0;
  #pragma omp parallel num_threads(3600)
  {
    #pragma omp atomic
    nb_threads++;
  }
  printf("nb_threads = %zu\n", nb_threads);
}

int main() {
  nb_thread_kernel();
  return 0;
}
