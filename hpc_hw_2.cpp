#include <vector>
#include <cmath>
#include <iostream>


int main(){
    int n{8};
    int divisor_length = static_cast<int>(std::log2(n));
    int divisors[divisor_length];
    int count_0_recv{0};
    int count_4_recv{0};
    for (int k=0; k<divisor_length; k++){
        divisors[k] = std::pow(2,k+1);
    }
    std::cout<<"Divisor length: "<<divisor_length<<std::endl;
    for (int i=0; i<divisor_length; i++){
        std::cout<<"Round "<<i+1<<std::endl;
        int divisor = divisors[i];
        int core_dce = std::pow(2,i);
        for (int j=0; j<n ; j++){
            if (j%divisor==core_dce) std::cout<<"Rank "<< j<< " sending to "<< j-core_dce<<std::endl;
            else if (j%divisor==0) {
                std::cout<<"Rank "<< j<< " receiving from "<< j+core_dce<<std::endl;
                std::cout<<"Rank "<< j<< " summing up"<<std::endl;
                if (j==0) count_0_recv++;
                else if (j==4) count_4_recv++;
            }
        }
    }
    std::cout<<"Master core is summing up"<<std::endl;

    std::cout<<"Rank 0 recieved: "<<count_0_recv<<std::endl;
    std::cout<<"Rank 4 recieved: "<<count_4_recv<<std::endl;

    return 0;
}