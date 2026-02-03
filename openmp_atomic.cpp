#include <cassert>
#include <iostream>
int main(){
    // Set up the number of iterations to run 
    const int numIterations = 1<<20;
    const int numThreads = 4;
    const int iterationsPerThread = numIterations / numThreads;
    int sink=0;
    // Hereis here we implement openmp
    #pragma omp parallel num_threads(4)
    {
        for (int i=0; i<iterationsPerThread;i++){
            #pragma omp atomic
            sink++; 
        }
    }
    std::cout<<sink<<std::endl;
    std::cout<<numIterations<<std::endl;


    assert(sink==numIterations);
    return 0;
}