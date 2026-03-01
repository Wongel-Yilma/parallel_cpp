#include <stdio.h>
#include <stdlib.h>


int main(){
    int size_max = 4;
    double A[size_max][size_max];
    double y[size_max];
    double x[size_max];
    srand(200);
    for (int i=0; i<size_max ; i++){
        for (int j=0; j<size_max; j++){
            if (j>=i) A[i][j] = rand()%10+1;
            else A[i][j] = 0;
            printf("%f ",A[i][j]);
        }
        y[i] = rand()%50;
        printf("   %f \n", y[i]);
    }
    int row, col;
    for (row=size_max-1; row>=0; row--){
        x[row] = y[row];
        for (col=row+1; col<size_max; col++){
            x[row]-=A[row][col]*x[col];
        }
        x[row]/=A[row][row];
        printf("%f \n",x[row]);
    }

    return 0;
}