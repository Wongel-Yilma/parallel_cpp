#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>


void Get_input(int , int , int, char**, int* , int* , double* , double* );
void Generate_data(int , int  , double* ,int ,int , double , double );
void get_bin_limits(int, double* ,double , double , int );
void  bin_data(double* , double* , int* , int , int );
int main(int argn, char* argv []){
    int my_rank,comm_sz;
    int bin_count, data_count, local_count;
    double min_meas, max_meas;
    double* local_data, *bin_limits;
    int* local_hist, *global_hist;
    global_hist = NULL;
    
    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
    Get_input(my_rank,comm_sz,argn, argv, &bin_count, &data_count, &min_meas, &max_meas);
    
    if (my_rank==0){
        global_hist = calloc(bin_count,sizeof(int));
    }
    local_count = data_count/comm_sz;
    local_data = malloc(local_count*sizeof(double));
    bin_limits = malloc((bin_count+1)*sizeof(double));
    local_hist = calloc(bin_count,sizeof(int));

    Generate_data( my_rank,  comm_sz ,  local_data, local_count, data_count,  min_meas,  max_meas);
    get_bin_limits(my_rank,bin_limits, min_meas, max_meas, bin_count);
    bin_data( local_data,  bin_limits,  local_hist, local_count,  bin_count);

    MPI_Reduce(local_hist, global_hist, bin_count, MPI_INT, MPI_SUM,0, MPI_COMM_WORLD);
    int j;
    if (my_rank==0){
        for ( j=0; j<bin_count;j++){
            printf("%d ",global_hist[j]);
        }
        printf("\n");
    }
    
    
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


void Generate_data(int my_rank, int comm_sz , double* local_data,int local_count,int data_count, double min_meas, double max_meas){
    double* full_data =NULL;
    int i;
    if (my_rank==0){
        full_data = malloc(data_count*sizeof(double));
        srand(100);
        for ( i=0; i<data_count; i++){
            full_data[i] = min_meas + (max_meas-min_meas)*(double)rand()/(RAND_MAX);
        }
    }
    MPI_Scatter(full_data, local_count, MPI_DOUBLE, local_data, local_count, MPI_DOUBLE, 0, MPI_COMM_WORLD);
}



void get_bin_limits(int my_rank,double* bin_limits,double min_meas, double max_meas, int bin_count){
    double interval;
    int i;
    if (my_rank==0){
        interval = (max_meas-min_meas)/(bin_count);
        for ( i=0; i<=bin_count; i++){
            bin_limits[i] = min_meas+ interval*i;
        }
    }
    MPI_Bcast(bin_limits, bin_count+1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
}



void  bin_data(double* local_data, double* bin_limits, int* local_hist, int local_count, int bin_count){
    int i,j;
    // Computing local histogram
    for ( i =0; i<local_count; i++){
        for ( j=1; j<bin_count+1;j++){
            if (local_data[i]<bin_limits[j]){
                local_hist[j-1]++;
                break;
            }
        }
    }
}
