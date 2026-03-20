#include <stdio.h>
#include <stdlib.h>
#include <mpi.h>


void Get_input(int , int , int, char**, int* , int* , double* , double* );
void Generate_data(int , int  , double** ,int* ,int , double , double );
void get_bin_limits(int, double* ,double , double , int );
void  bin_data(double* , double* , int* , int , int );


int main(int argn, char* argv []){
    // Variable and array definition
    int my_rank,comm_sz;                        // MPI variables
    int bin_count, data_count, local_count;     // histogram variables: data count-> Total , local_count-> number of data per MPI rank
    double min_meas, max_meas;                  // Minimum and Maximum values of the data array to be generated
    
    // Arrays  
    double* local_data, *bin_limits;           
    int* local_hist, *global_hist;
    global_hist = NULL;             
    
    // Initializing MPI and getting the total comm size and rank of each processor
    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);
    MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);

    // Getting the input from the command line args
    Get_input(my_rank,comm_sz,argn, argv, &bin_count, &data_count, &min_meas, &max_meas);
    
    // We only need to accumulate the final bin count in rank 0.
    if (my_rank==0){
        global_hist = calloc(bin_count,sizeof(int));
    }
    
    // Initializing local histogram with zero values
    local_hist = calloc(bin_count,sizeof(int));
    
    // Generate and distribute the data
    Generate_data( my_rank, comm_sz , &local_data, &local_count, data_count, min_meas, max_meas);
    bin_limits = malloc((bin_count+1)*sizeof(double));
    get_bin_limits(my_rank, bin_limits, min_meas, max_meas, bin_count);
    bin_data( local_data, bin_limits, local_hist, local_count, bin_count);

    MPI_Reduce(local_hist, global_hist, bin_count, MPI_INT, MPI_SUM,0, MPI_COMM_WORLD);
    int j;
    if (my_rank==0){
        printf("bin_maxes: ");
        for ( j=1; j<bin_count+1;j++){
            printf("%f ",bin_limits[j]);
        }
        printf("\n");
        printf("bin_counts: ");
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
        *min_meas_p = strtof(argv[2], NULL);
        *max_meas_p = strtof(argv[3], NULL);
        *data_count_p = strtol(argv[4], NULL, 10);
    }
    MPI_Bcast(bin_count_p, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(data_count_p, 1, MPI_INT, 0, MPI_COMM_WORLD);
    MPI_Bcast(min_meas_p, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    MPI_Bcast(max_meas_p, 1, MPI_DOUBLE, 0, MPI_COMM_WORLD);
}


void Generate_data(int my_rank, int comm_sz , double** local_data_p,int *local_count_p,int data_count, double min_meas, double max_meas){
    double* full_data =NULL;
    int i;
    if (my_rank==0){
        full_data = malloc(data_count*sizeof(double));
        srand(100);
        for ( i=0; i<data_count; i++){
            full_data[i] = min_meas + (max_meas-min_meas)*(double)rand()/(RAND_MAX);
        }
    }
    // Handling the case in which total number of data is not equally divisible among workers
    int *displacements= calloc(comm_sz,sizeof(int));
    int *scounts = calloc(comm_sz,sizeof(int));
    int rmdr = data_count%comm_sz; // Calculating the remainder 
    int sum = 0;
    for (i=0; i<comm_sz; i++){
        scounts[i] = (data_count / comm_sz) + (i < (rmdr) ? 1 : 0);    // Distributing the remainder to ranks 0 upto rank rmdr-1
        displacements[i] = sum;   
        sum+=scounts[i];
    }
    // Assigning the size of local_data using pointers
    *local_count_p = scounts[my_rank];
    // Define the size of the local data using the calculated size
    *local_data_p = malloc((*local_count_p)*sizeof(double));

    // Scatterv is capable of distributing variable sized chunks to different ranks.
    MPI_Scatterv(full_data, scounts,displacements,  MPI_DOUBLE, *local_data_p, *local_count_p, MPI_DOUBLE, 0, MPI_COMM_WORLD);

    // MPI_Scatter(full_data, *local_count_p,  MPI_DOUBLE, local_data, *local_count_p, MPI_DOUBLE, 0, MPI_COMM_WORLD);
    free(displacements);
    free(scounts);
    if (my_rank == 0) free(full_data);
}



void get_bin_limits(int my_rank,double* bin_limits,double min_meas, double max_meas, int bin_count){
    double interval;
    int i;
    // Calculating the bin limits by equaly dividing the whole domain among bin-counts
    if (my_rank==0){
        interval = (max_meas-min_meas)/(bin_count);
        for ( i=0; i<=bin_count; i++){
            bin_limits[i] = min_meas+ interval*i;
        }
    }
    // Then Broadcasting the limits to all processors
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
