#include <iostream>
#include <vector>
#include <tuple>
#include <cmath>


std::tuple<std::vector<int>, std::vector<int>> calculate_first_last (int n, int p){
    int el_per_core = std::ceil(static_cast<double>(n)/static_cast<double>(p));
    std::cout<<"Element per core: "<< el_per_core<<std::endl;
    std::vector<int> my_first_i (p,0);
    std::vector<int> my_last_i (p,0);
    for (int j =0; j< p; j++){
        if (j!=p-1){
            my_first_i[j] = j*el_per_core;
            my_last_i[j] = (j+1)*el_per_core-1;
        }
        else{
            my_first_i[j] = j*el_per_core;
            my_last_i[j] = n-1;
        }
    }
    return std::make_tuple(my_first_i, my_last_i);
}

int main() {
    int n =13;
    int p =4;
    auto [my_first_i, my_last_i] =  calculate_first_last(n,p);

    for (int i = 0; i<static_cast<int>(n/p)+1; i++){
        std::cout<<"Rank "<<i<<std::endl;
        std::cout<<my_first_i[i]<<std::endl;
        std::cout<<my_last_i[i]<<std::endl;
    }
    return 0;
}