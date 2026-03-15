#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>

void Generate_vector(double*, int , int, int , MPI_Comm);
void Add_vectors(double*, double*, double*, int);
void Print_full_vector(int ,double* , int ,int  );
int main(){
    int my_rank, comm_sz, n = 8 , local_n,i;
    double* local_a;
    double* local_b;
    double* local_c;
    MPI_Init(NULL, NULL);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    local_n = n/comm_sz;
    local_a = malloc(local_n*sizeof(double));
    local_b = malloc(local_n*sizeof(double));
    local_c = malloc(local_n*sizeof(double));

    Generate_vector(local_a, local_n, n, my_rank, MPI_COMM_WORLD);
    Generate_vector(local_b, local_n, n, my_rank, MPI_COMM_WORLD);
    Add_vectors(local_a, local_b, local_c, local_n);
    MPI_Barrier(MPI_COMM_WORLD);
    Print_full_vector(my_rank, local_c, local_n, n);

    printf("Rank %d has %d number of elements\n", my_rank, local_n);

    MPI_Finalize();
    return 0;
}
void Generate_vector(double* local_a, int local_n, int n, int my_rank, MPI_Comm comm){
    double* a =NULL;
    int i ;
    if (my_rank==0){
        a = malloc(n*sizeof(double));
        for (i=0; i<n; i++){
            a[i] = (double)rand()/(RAND_MAX);
        }
        printf("Rank %d has created %d number of elements\n", my_rank,n);
    }
    MPI_Scatter(a, local_n, MPI_DOUBLE, local_a,local_n, MPI_DOUBLE, 0, comm);
    free(a);
}
void Add_vectors(double* a, double* b, double* c, int n){
    int k ;
    for (k=0; k<n; k++){
        c[k] = a[k]+b[k];
    }
    
}

void Print_full_vector(int my_rank,double* local_c, int local_n,int n ){
    double* b = NULL;
    int k;
    if (my_rank==0){
        b = malloc(n*sizeof(double));
    }
    MPI_Gather(local_c, local_n, MPI_DOUBLE, b, local_n, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    if (my_rank==0){
        for (k = 0; k < n; k++)
            printf("%f ", b[k]);
    }
    free(b);

}

