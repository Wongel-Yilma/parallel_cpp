#include <iostream>
#ifdef _OPENMP
#include <omp.h>
#endif
#include <random>
#include <algorithm>

// void Swap(int*, int*);

int main(){
    std::random_device rd;
    std::mt19937 gen(rd());
    int n = 20;
    std::uniform_int_distribution<int> dist (1,100);
    std::vector<int> array(n);
    std::generate(array.begin(), array.end(), [&](){return dist(gen);});
    for (int i =0; i<n; i++){
        std::cout<<" "<<array[i];
    }
    std::cout<<std::endl;

    int  i, temp, phase;

    #pragma omp parallel num_threads(4) default(none) shared(n,array) private(i,temp, phase)
    for ( phase=0; phase<n; phase++){
        if (phase%2==0){
            #pragma omp for
            for (i=1; i<n; i+=2){
                if (array[i-1]>array[i]) {
                    temp = array[i-1];
                    array[i-1] = array[i];
                    array[i] = temp;
                }
            }
        }
        else {
            #pragma omp for
            for (i=1; i<n-1; i+=2){
                if (array[i]>array[i+1]) {
                    temp = array[i];
                    array[i] = array[i+1];
                    array[i+1]=temp;
                }
            }
        }
    }


    for (int i =0; i<n; i++){
        std::cout<<" "<<array[i];
    }
    std::cout<<std::endl;
    return 0;
}

