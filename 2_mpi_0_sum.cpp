#include <algorithm>
#include <iostream>
#include <memory>
#include <random>

#include "mpi.h"

int main(int argc , char *argv[]){
    MPI_Init(&argc, &argv);
    int rank,size;
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    const int numElements = 1 <<10 ;
    const int chunkSize = numElements/size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    // Create buffer for sending
    std::unique_ptr<int []> send_ptr;

    if (rank==0){
        send_ptr = std::make_unique<int []>(numElements);
        
        // Creating a random number generator
        std::random_device rd;
        std::mt19937 mt(rd());
        std::uniform_int_distribution dist(1,100);
        // Creating a random data
        std::generate(send_ptr.get(), send_ptr.get()+numElements,[&]{return dist(mt);});

    }
    auto recv_buffer = std::make_unique<int  []> (chunkSize);
    MPI_Scatter(send_ptr.get(), chunkSize, MPI_INT, recv_buffer.get(),chunkSize, MPI_INT, 0, MPI_COMM_WORLD);
    // Each process computes local sum
    auto local_sum = std::reduce(recv_buffer.get(), recv_buffer.get()+chunkSize, 0);
    // Reduce all local sums to global sum
    int global_sum;
    MPI_Reduce(&local_sum, &global_sum, 1, MPI_INT, MPI_SUM, 0, MPI_COMM_WORLD);
    if (rank==0){
        std::cout<<"Global sum is "<<global_sum <<"\n";
    }
    MPI_Finalize();
    return 0;
}