#include <iostream>
#include <random>
#include <pthread.h>

struct ThreadArgs{
    int rank; 
    int number_tosses;
    double* global_pi;
    pthread_mutex_t* mutex;
};

int thread_count;
void* calc_pi(void* args);

int main(int argn, char* argv []){  
    // thread_count = std::stoi(argv[1]);
    thread_count = 2;
    long number_tosses = 10000;
    double global_pi =0.0;
    // pthread_t* thread_handles;
    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, NULL);
    // thread_handles = (pthread_t*)malloc(thread_count*sizeof(pthread_t));
    pthread_t* thread_handles = new pthread_t[thread_count];
    ThreadArgs* thread_args = new ThreadArgs[thread_count];
    for (int thread=0; thread<thread_count; thread++){
        thread_args[thread].rank=thread;
        thread_args[thread].number_tosses = number_tosses;
        thread_args[thread].global_pi = &global_pi;
        thread_args[thread].mutex = &mutex;

        pthread_create(&thread_handles[thread], NULL, calc_pi, (void*)&thread_args[thread]);
    }
    for (int thread=0; thread<thread_count; thread++){
        pthread_join(thread_handles[thread], NULL);
    }
    std::cout<<"PI approximation: "<< global_pi <<std::endl;
    return 0;   
}

void* calc_pi(void * thread_args){
    ThreadArgs* args = (ThreadArgs*) thread_args;
    long number_tosses = args->number_tosses;
    long my_rank =  args->rank;
    double* global_pi = args->global_pi;
    pthread_mutex_t* mutex= args->mutex;
    long number_in_circle = 0;
    double x, y;
    double distance = 0.0;
    std::mt19937 gen(my_rank);
    std::uniform_real_distribution<double> distrib(-1.0,1.0);
    
    for (int i=0; i<number_tosses; i++){
        x = distrib(gen);
        y = distrib(gen);
        distance = x*x + y*y;
        if (distance<=1) number_in_circle++;
    }
    double pi = 4*(double)(number_in_circle)/(double)number_tosses;
    std::cout<<my_rank<<" :"<<pi<<std::endl;
    pthread_mutex_lock(mutex);
    *global_pi+= pi;
    pthread_mutex_unlock(mutex);

    return NULL;
}