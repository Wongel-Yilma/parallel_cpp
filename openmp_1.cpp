#include <benchmark/benchmark.h>
#include <random>
#include <vector>


static void baseline(benchmark::State &s){
    // Create a random number gnerator
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<float> dist (0.0f, 1.0f);
    // Create a vector of random numbers
    const int numElements = 1<<20;
    std::vector<float> v_in ;
    std::generate_n(std::back_inserter(v_in), numElements,[&]{return dist(mt);});
    // Output vector
    std::vector<float> v_out(numElements);

    // Timing loop
    for (auto _:s){
        #pragma omp parallel for
        for (int i=0; i<numElements; i++){
            v_out[i] = v_in[i] * v_in[i];
        }

    }
    
}

BENCHMARK(baseline)->Unit(benchmark::kMicrosecond)->UseRealTime();
BENCHMARK_MAIN();