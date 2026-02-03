#include <iostream>
#include <vector>
#include <tuple>
#include <cmath>


std::tuple<std::vector<int>, std::vector<int>, std::vector<int>, std::vector<int>> calculate_first_last (int n, int p){
    int el_per_core_front = std::ceil(static_cast<double>(n)/static_cast<double>(p))/2;
    int el_per_core_back = std::ceil(static_cast<double>(n)/static_cast<double>(p)) - el_per_core_front;
    int last_back = std::ceil(static_cast<double>(n)/2);

    std::cout<<"Element per core: "<< el_per_core_front<<  "   "<< el_per_core_back  <<std::endl;
    std::vector<int> my_first_i_front (p,0);
    std::vector<int> my_last_i_front (p,0);
    std::vector<int> my_first_i_back (p,0);
    std::vector<int> my_last_i_back (p,0);
    for (int j =0; j< p; j++){
        
        my_first_i_front[j] = j*el_per_core_front;
        my_last_i_front[j] = (j+1)*el_per_core_front-1;
        if (j!=p-1){
            my_last_i_back[j] = n-j*el_per_core_front-1;
            my_first_i_back[j] = n-(j+1)*el_per_core_front;
        }
        else{
            my_last_i_back[j] = n-j*el_per_core_front-1;
            my_first_i_back[j] = last_back;
        }
    }
    return std::make_tuple(my_first_i_front, my_last_i_front,my_first_i_back,my_last_i_back);
}

int main() {
    int n =23;
    int p =4;
    auto [my_first_i_front, my_last_i_front,my_first_i_back, my_last_i_back] =  calculate_first_last(n,p);

    for (int i = 0; i<p; i++){
        std::cout<<"Rank "<<i<<std::endl;
        std::cout<<my_first_i_front[i]<<std::endl;
        std::cout<<my_last_i_front[i]<<std::endl;
        std::cout<<my_first_i_back[i]<<std::endl;
        std::cout<<my_last_i_back[i]<<std::endl;
    }
    return 0;
}