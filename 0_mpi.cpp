#include <iostream>
#include <sstream>
#include "mpi.h"

int main(int argc, char *argv[]){
    MPI_Init(&argc, &argv);
    int rank, size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    std::ostringstream ss;
    ss<<"Hello from process " << rank << " of " << size << std::endl;
    std::cout << ss.str();
    MPI_Finalize();

    return 0;
}
