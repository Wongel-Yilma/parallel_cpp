#include <iostream>
#include <pthread.h>
#include <cstdio>
#include <cstdlib>

int thread_count;
void* Hello (void* rank);

int main(int argc, char* argv[]){
    long thread;
    pthread_t* thread_handles;
    // Get Number of threads from command line
    thread_count = strtol(argv[1], NULL, 10);
    thread_handles = (pthread_t*)malloc(thread_count*sizeof(pthread_t));

    for (thread=0; thread<thread_count; thread++){
        printf("Thread: %ld \n",thread);
        pthread_create(&thread_handles[thread], NULL, Hello, (void*)thread);
    }
    printf("Hello from the main thread\n");
    for (thread=0; thread<thread_count; thread++){
        pthread_join(thread_handles[thread], NULL);
    }
    free(thread_handles);
    return 0;
}


void* Hello(void * rank){
    long my_rank = (long)rank;
    printf("Hello from thread %ld of %d \n", my_rank, thread_count);
    return NULL;
}