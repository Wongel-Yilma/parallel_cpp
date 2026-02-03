#include <vector>
#include <iostream>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>
#include <random>
int thread_count;
void *mat_mul(void * rank);
std::vector<std::vector<double>> A;
std::vector<double> x,y;
int MAX;

int main(int argn, char* argv[]){
    MAX = 100;
    A.resize(MAX, std::vector<double>(MAX));
    y.resize(MAX);
    x.resize(MAX);
    int thread;
    pthread_t* thread_handles;
    thread_count = strtol(argv[1], NULL, 10);
    thread_handles = (pthread_t *)malloc(thread_count*sizeof(pthread_t));
    std::random_device rnd_device;
    std::mt19937 mersenne_engine{rnd_device()};
    std::uniform_real_distribution<double> dist{0.0, 1.0};
    auto generate_func =[&](){return dist(mersenne_engine);};
    std::generate(x.begin(), x.end(), generate_func);

    for (auto & row:A){
        std::generate(row.begin(), row.end(), generate_func);
    }
    for (thread = 0; thread<thread_count ;thread++){
        pthread_create(&thread_handles[thread],NULL, mat_mul, (void*)thread);
    }
    for (thread = 0; thread<thread_count ;thread++){
        pthread_join(thread_handles[thread],NULL);
    }
    for (int i=0; i<MAX;i++){
        printf("%f\n",y[i]);
    }
    free(thread_handles);
    return 0;

}

void * mat_mul(void * rank){
    long my_rank = (long) rank;
    int local_m = MAX/thread_count;
    int first_row = my_rank*local_m;
    int last_row = (my_rank+1)*local_m;
    int i,j;
    for (int i=first_row; i<last_row; i++){
        for (int j=0; j<MAX; j++){
            y[i]+=A[i][j]*x[j];
        }
    }
    printf("Rank %ld finished calculation.\n", my_rank);
    
    return NULL;
}