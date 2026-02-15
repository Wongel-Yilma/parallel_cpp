#include <stdio.h>
#include <stdlib.h>
#ifdef _OPENMP
#include <omp.h>
#endif

void Hello(void);

int main(int argn, char* argv[]){
    int thread_count = strtol(argv[1], NULL,10);

#pragma omp parallel num_threads(thread_count)
    Hello();
    return 0;
}

void Hello(void){
#ifdef _OPENMP
    int my_rank = omp_get_thread_num();
    int total_rank = omp_get_num_threads();
#else
    int my_rank = 0;
    int total_rank = 1;
#endif
    printf("Hello, from rank %d out of %d\n", my_rank, total_rank);
}
