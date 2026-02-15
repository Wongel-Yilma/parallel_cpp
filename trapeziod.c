#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#ifdef _OPENMP
#include <omp.h>
#endif

double f(double);
void Trapz(int a , int b, int n, int* global_approx_ptr);

int main(){
    int thread_count =2; 
    double a = 0.0;
    double b = 3.1415;
    int n=50000;
    double global_approx;
#pragma omp parallel for num_threads(thread_count)
    Trapz(a,b,n,&global_approx);
    
    printf("Aprroximation: %f \n", global_approx);
    return 0;
}

double f(double x){
    return sin(x);
}
void Trapz(int a , int b, int n, int* global_approx_ptr){

    double h, local_approx;
    double local_a, local_b;
    int i, local_n;
    double h = (b-a)/n;
    int my_rank  = get_thread_num();
    int num_threads = get_num_threads();
    local_n = n/num_threads;
    local_a = a+local_n*my_rank*h;
    local_b = a+local_n*(my_rank+1)*h;
    local_approx = (f(local_a)+f(local_b))/2.0;
    



}
