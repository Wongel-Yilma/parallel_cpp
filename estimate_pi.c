#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#ifdef _OPENMP
#include <omp.h>
#endif

int main(){
    int MAX_N = 100000;

    double pi = 0;
    int i;
    double factor = 1.0;
#pragma omp parallel for num_threads(8) private(i, factor)  shared( MAX_N) reduction(+:pi) 
    for ( i = 0; i<MAX_N; i++){
        factor = (i%2==0)? 1.0 : -1.0;
        pi+=factor/(2*i+1);
    }
    pi*=4;
    printf("Pi: %f\n", pi);

    return 0;
}

