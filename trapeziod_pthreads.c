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
    double *global_sum;
    pthread_mutex_t* mutex;
};

long thread_count;
int main (){
    thread_count = 8;
    int n = 10000;
    double a = 0;
    double b = 5;
    double global_sum = 0.0;
    pthread_t* thread_handles;
    thread_handles = (pthread_t *)malloc(thread_count* sizeof(pthread_t));
    struct Thread_args* thread_arguments;
    thread_arguments = (struct Thread_args* ) malloc(thread_count*sizeof(struct Thread_args));
    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, NULL);
    int thread;
    for (thread=0; thread<thread_count; thread++){
        thread_arguments[thread].n = n;
        thread_arguments[thread].a = a;
        thread_arguments[thread].b = b;
        thread_arguments[thread].thread_rank=thread;
        thread_arguments[thread].global_sum=&global_sum;
        thread_arguments[thread].mutex = &mutex;
        pthread_create(&thread_handles[thread], NULL, Trapz, &thread_arguments[thread]);
    }
    
    for (thread=0; thread<thread_count; thread++){
        pthread_join(thread_handles[thread], NULL);
    }

    printf("Aprroximation: %f \n", global_sum);
    pthread_mutex_destroy(&mutex);

    return 0;
}


double f(double x){
    return x;
}

void* Trapz(void* Args){
    struct Thread_args* arguments = (struct Thread_args* )Args;
    int thread = arguments->thread_rank;
    int n = arguments->n;
    double a = arguments->a;
    double b = arguments->b;
    double *global_sum = arguments->global_sum;
    pthread_mutex_t* mutex = arguments->mutex;


    double h = (b-a)/n;
    int local_n = n/thread_count;
    double local_a = a+h*local_n*thread;
    double local_b = local_a + h*local_n;

    double local_sum = (f(local_a)+f(local_b))/2;
    for (int i=1; i<local_n; i++){
        local_sum += f(local_a+h*i);
    }
    local_sum*=h;
    pthread_mutex_lock(mutex);
    *global_sum+=local_sum;
    pthread_mutex_unlock(mutex);
        
    return NULL;
}