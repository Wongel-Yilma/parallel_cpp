#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>


void Get_input(int , int , int, char**, int* , int* , double* , double* );

int main(int argn, char* argv []){
    int my_rank,comm_sz;
    int bin_count, data_count;
    double min_meas, max_meas;
    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    Get_input(my_rank,comm_sz,argn, argv, &bin_count, &data_count, &min_meas, &max_meas);

    printf("Rank %f recieved bin count num \n",min_meas);
    MPI_Finalize();
    
    return 0;
}
void Get_input(int my_rank, int comm_sz, int argn, char** argv, int* bin_count_p, int* data_count_p, double* min_meas_p, double* max_meas_p){
    if (my_rank==0){
        if (argn!=5){
            fprintf(stderr,"Number of arguments must be 4");
            exit(EXIT_FAILURE);
        }
        *bin_count_p = strtol(argv[1], NULL, 10);
        *data_count_p = strtol(argv[2], NULL, 10);
        *min_meas_p = strtof(argv[3], NULL);
        *max_meas_p = strtof(argv[4], NULL);
    }
    MPI_Bcast(bin_count_p, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(data_count_p, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(min_meas_p, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(max_meas_p, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);

}
