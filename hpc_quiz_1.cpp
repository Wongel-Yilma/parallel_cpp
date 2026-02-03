#include <vector>
#include <cmath>
#include <iostream>

std::vector<double> get_comp_time(std::vector<std::vector<int>>& arr){
    std::vector<double> comp_time (arr.size(),0);
    for (int i=0; i<arr.size(); i++){
        // Loop over the elements to calculate their computation time
        for (int j=0; j<arr[0].size(); j++){
            comp_time[i]+= arr[i][j] ;
        }
    }

    return comp_time;
}

std::vector<std::vector<int>> assign_indices (int n, int p ){
    // Calculate the average elements per core
    int elts_per_core = std::ceil(n/p);
    std::vector<std::vector<int>> two_dim_idx (p,std::vector<int>(elts_per_core,0));
    // Outer loop over the p processors 
    for (int i=0; i<p; i++){
    // Inner loop over elts_per_core
        for (int j=0; j<elts_per_core; j++){
            two_dim_idx[i][j] = i+j*p;
        }
    }
    return two_dim_idx;
}

int main(){
    std::vector<std::vector<int>> two_dim_idx = assign_indices(15,3);
    std::vector<double> comp_time  = get_comp_time(two_dim_idx);

    for (int i=0; i<two_dim_idx.size(); i++){
        // Inner loop over elts_per_core
            for (int j=0; j<two_dim_idx[0].size(); j++){
                std::cout<< two_dim_idx[i][j]<<std::endl;
            }
            std::cout<<"Comp time for core "<<i<<" is "<<comp_time[i]<<std::endl;
        }
    return 0;
}