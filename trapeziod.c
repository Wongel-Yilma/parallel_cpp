#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <errno.h>
#ifdef _OPENMP
#include <omp.h>
#endif

double f(double);
void Trapz(double a , double b, int n, double* global_approx_ptr);

int main(int argn , char *argv[]){
    if (argn!=5){
        perror("Number of arguments is incorrect!");
        exit(EXIT_FAILURE);
    }
    int thread_count = strtol(argv[1], NULL, 10); 
    double a = strtof(argv[2], NULL); 
    double b = strtof(argv[3], NULL); 
    int n = strtol(argv[4], NULL, 10); 
    double global_approx;
#pragma omp parallel num_threads(thread_count)
    Trapz(a,b,n,&global_approx);
    
    printf("Aprroximation: %f \n", global_approx);
    return 0;
}

double f(double x){
    // return cos(x);
    return x;
}
void Trapz(double a , double b, int n, double* global_approx_ptr){
#ifdef _OPENMP
    int my_rank  = omp_get_thread_num();
    int num_threads = omp_get_num_threads();  
#else
    int my_rank  = 0;
    int num_threads = 1;    
#endif

    double h, local_approx;
    double local_a, local_b;
    int i, local_n;
    h = (b-a)/n;
    local_n = n/num_threads;
    local_a = a+local_n*my_rank*h;
    local_b = local_a + local_n*h;
    local_approx = (f(local_a)+f(local_b))/2.0;
    for (i=1; i<local_n-1; i++){
        local_approx+=f(local_a+i*h);
    }
    local_approx *=h;
    // printf("local approx: %f\n", local_approx);
    // printf("local a and b: %f %f\n", local_a, local_b);
    // printf("ranks %d %d\n", my_rank, num_threads);


#pragma omp critical
    *global_approx_ptr+=local_approx;
    
}
