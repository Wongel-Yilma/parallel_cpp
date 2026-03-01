#include <stdio.h>
#include <stdlib.h>


int main(){
    int size_max = 100;
    double A[size_max][size_max];
    double y[size_max];
    double x[size_max];
    srand(200);
    for (int i=0; i<size_max ; i++){
        for (int j=0; j<size_max; j++){
            // if (j>=i) A[i][j] = rand()%10+1;
            // else A[i][j] = 0;
            A[i][j] = rand()%10+1;
            // printf("%f ",A[i][j]);
        }
        y[i] = rand()%50;
        // printf("   %f \n", y[i]);
    }
    int row, col, phase;
    double factor;
    #pragma omp parallel num_threads(2) default(none) private(phase, col, row) shared(A,size_max, factor)
    for (phase=0; phase<size_max-1; phase++){
        for(row=size_max-1; row>phase; row--){
            #pragma omp single
            {
                factor = A[row][phase]/A[phase][phase];
            }
            #pragma omp for
            for (col=phase; col<size_max; col++){
                A[row][col] -= factor*A[phase][col];
            }
            #pragma omp single
            {
                y[row]-=factor*y[phase];
            }
        }
    }

    // for (int i=0; i<size_max ; i++){
    //     for (int j=0; j<size_max; j++){
    //         printf("%f ",A[i][j]);
    //     }
    //     printf("    %f\n", y[i]);
    // }
    double sum;

    #pragma omp parallel num_threads(6) default(none) private(row, col) shared(x, A, y, size_max, sum) 
    for (row=size_max-1; row>=0; row--){
        // #pragma omp single
        // {
        //     sum= -y[row];
        // }
        sum = 0.0;
        #pragma omp for reduction(+:sum) schedule(runtime)
        for (col=row+1; col<size_max; col++){
            sum+=A[row][col]*x[col];
        }
        #pragma omp single
        {   
            x[row]= (y[row]-sum)/A[row][row];
        }
    }

    for (row=0; row<10; row++)
        printf("%f ",x[row]);
    printf("\n ");

    return 0;
}