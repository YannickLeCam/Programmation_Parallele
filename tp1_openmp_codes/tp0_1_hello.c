#include <stdio.h>
#include <omp.h>
int main() {
  #pragma omp parallel num_threads(19)
  {
    printf("Hello\n");
    printf(" le numéro de est : %d\n", omp_get_thread_num());
  }
  
  printf("World\n");
  return 0;
}
