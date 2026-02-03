#include <cassert>
#include <iostream>

int main(){
    // Set up the number of iterations to run 
    const int numIterations = 1<<20;
    const int numThreads = 8;
    const int iterationsPerThread = numIterations / numThreads;
    int sink=0;
    // Hereis here we implement openmp
    #pragma omp parallel num_threads(8)
    {
        for (int i=0; i<iterationsPerThread;i++){
            #pragma omp critical
            sink++;
        }
    }
    std::cout<<sink<<std::endl;
    std::cout<<numIterations<<std::endl;

    assert(sink==numIterations);
    return 0;
}