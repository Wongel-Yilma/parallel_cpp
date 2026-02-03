#include <iostream>
#include <sstream>
#include "mpi.h"

int main(int argc, char *argv[]){
    MPI_Init(&argc, &argv);
    int rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    // Rank 0 coordinates work
    if (rank==0){
        int size;
        MPI_Comm_size(MPI_COMM_WORLD, &size);
        for (int i=1;i<size; i++){
            MPI_Send(&i, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
        }
        // Receive results
        for (int i=1; i<size; i++){
            int recv;
            MPI_Recv(&recv, 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            std::stringstream ss;
            ss<< "Recieved "<<recv <<" from rank "<<i <<"\n";
            std::cout<<ss.str();
        }
    }
    else {
        // Recieve work from rank 0
        int recv;
        MPI_Recv(&recv, 1, MPI_INT, 0,0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        recv*=recv;
        MPI_Send(&recv, 1, MPI_INT, 0,0, MPI_COMM_WORLD);

    }
    MPI_Finalize();

    return 0;
}