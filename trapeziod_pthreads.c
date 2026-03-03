#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <errno.h>
#include <pthread.h>


double f(double);
void* Trapz (void*);

struct Thread_args {
    int n;
    double a;
    double b;
    int thread_rank;
};

long thread_count;
int main (){
    thread_count = 4;
    int n = 50;
    double a = 0;
    double b = 5;
    pthread_t* thread_handles;
    thread_handles = (pthread_t *)malloc(thread_count* sizeof(pthread_t));
    struct Thread_args* thread_arguments;
    int thread;
    for (thread=0; thread<thread_count; thread++){
        thread_arguments[thread].n = n;
        thread_arguments[thread].a = a;
        thread_arguments[thread].b = b;
        thread_arguments[thread].thread_rank=thread;

        pthread_create(&thread_handles, NULL, Trapz, &thread_arguments[thread]);
    }
    
    return 0;
}


double f(double x){
    return x;
}

void* Trapz(void* Args){
    struct Thread_args* arguments = Args;
    int thread = arguments->thread_rank;
    int n = arguments->n;
    double a = arguments->a;
    double b = arguments->b;

    
    
}