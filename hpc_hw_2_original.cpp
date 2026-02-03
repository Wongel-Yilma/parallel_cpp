#include <vector>
#include <cmath>
#include <iostream>
#include <algorithm>
#include <random>

std::vector<std::vector<int>> assign_indices (int n, int p ){
    // Calculate the average elements per core
    int elts_per_core = std::ceil(n/p);
    std::vector<std::vector<int>> two_dim_idx (p,std::vector<int>(elts_per_core,0));
    // Outer loop over the p processors 
    int counter {0};
    for (int i=0; i<p; i++){
    // Inner loop over elts_per_core
        for (int j=0; j<elts_per_core; j++){
            // two_dim_idx[i][j] = i+j*p;
            two_dim_idx[i][j] =counter++;
        }
    }
    return two_dim_idx;
}


int main(){
    int n{2048};
    int p {4};
    std::vector<double> vec(n);
    std::random_device rnd_device;
    std::mt19937 mersenne_engine{rnd_device()};
    std::uniform_real_distribution<double> dist{0.0, 1.0};
    auto generate_func =[&](){return dist(mersenne_engine);};
    std::generate(vec.begin(), vec.end(), generate_func);
    std::vector<std::vector<int>>indices = assign_indices(n,p);
    std::vector<double>partial_sum(p,0.0);

    for (int i=0;i<indices.size(); i++){
        for (int j=0; j<indices[0].size(); j++){
            std::cout<<"Rank "<<i << " processing adding "<< indices[i][j]<<std::endl;
            partial_sum[i]+=vec[indices[i][j]];
        }
    }
    double total_sum {0};
    for (int k; k<p; k++){
        total_sum+=partial_sum[k];
    }
    std::cout<<"Total sum: "<<total_sum<<std::endl;

    return 0;
}