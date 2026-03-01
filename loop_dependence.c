#include <stdio.h>
#include <stdlib.h>

int main(){
    int serial_arr [21];
    int parallel_arr[21];
    serial_arr[0] = 0;
    parallel_arr[0] = 0;

    for (int i = 1; i< 21; i++){
        serial_arr[i] = serial_arr[i-1]+i;
    }
    int divisor, i;
#pragma omp parallel for num_threads(4) default(none) private(i,divisor) shared(parallel_arr)
    for ( i = 1; i< 21; i++){
        divisor = (i+1)/2;
        if ((i+1)%2==0) parallel_arr[i] = i*divisor;
        else parallel_arr[i] = i+parallel_arr[i-1];
    }
    // Print out the serial array
    for (int i = 0; i< 21; i++){
        printf("%d ", serial_arr[i]);
    }
    printf("\n");
    for (int i = 0; i< 21; i++){
        printf("%d ", parallel_arr[i]);
    }
    printf("\n");
    return 0;
}