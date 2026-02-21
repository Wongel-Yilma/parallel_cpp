#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#ifdef _OPENMP
#include <omp.h>
#endif

int main(){
    int MAX_N = 100000;

    double pi = 0;
    int i;
#pragma omp parallel num_threads(4) private(i)  shared( MAX_N) reduction(+:pi) 
    {
    double local_pi = 0.0;
#pragma omp for
    for ( i = 0; i<MAX_N; i++){
        local_pi+=pow(-1.0,i)/(2*i+1);
    }
    pi+=local_pi;
}
    pi*=4;
    printf("Pi: %f\n", pi);

    return 0;
}

