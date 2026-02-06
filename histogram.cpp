#include <cstdlib>
#include <cstdio>
#include <pthread.h>

int thread_count ;

struct ThreadArguments {
    int my_rank;
    int bin_count;
    int data_count;
    int* array;
    double* bin_limits;
    int* histogram;
    pthread_mutex_t* mutex;
};

double* get_bin_limits(int min, int max, int bin_count);
void*  bin_per_thread(void* arguments);
int main(){
    srand(100);
    thread_count = 4;
    const int array_size {100};
    int array [array_size];
    const int bin_count = 12;
    int max_val = 100;
    int min_val = 0;

    for (int j=0; j<array_size;j++) array[j]=rand()%(max_val-min_val)+min_val;
    int histogram[bin_count]={0};
    double* bin_limits = get_bin_limits(min_val, max_val,bin_count);
    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, NULL);
    long thread;    
    pthread_t* thread_handles;
    thread_handles = (pthread_t*)malloc(thread_count*sizeof(pthread_t));
    // Easier to use struct to pass data for each thread (more organized)
    ThreadArguments* thread_args = new ThreadArguments[thread_count];
    for (thread=0; thread<thread_count; thread++){
        thread_args[thread].my_rank = thread;
        thread_args[thread].bin_count = bin_count;
        thread_args[thread].data_count = array_size;
        thread_args[thread].array = array;
        thread_args[thread].bin_limits = bin_limits;
        thread_args[thread].histogram = histogram;
        thread_args[thread].mutex = &mutex;


        pthread_create(&thread_handles[thread], NULL, bin_per_thread,(void* )(&thread_args[thread]));
    }

    
    // serial implementation

    // for (int i = 0; i<array_size; i++){
    //     for (int j=1; j<bin_count+1;j++){
    //         if (array[i]<bin_limits[j]){
    //             histogram[j-1]++;
    //             break;
    //         }
    //     }
    // }

    for (thread=0; thread<thread_count; thread++){

        pthread_join(thread_handles[thread], NULL);
    }

    for (int j=0; j<bin_count;j++){
        printf("%d\n", histogram[j]);
    }


    delete[] bin_limits;
    delete[] thread_handles;
    delete[] thread_args;
    pthread_mutex_destroy(&mutex);
    return 0 ;
}

double* get_bin_limits(int min, int max, int bin_count){
    double* bin_limits = new double[bin_count+1];
    double interval = static_cast<double>(max-min)/static_cast<double>(bin_count);
    for (int i=0; i<=bin_count; i++){
        bin_limits[i] = min+ interval*i;
    }
    return bin_limits;
}
void*  bin_per_thread(void* arguments){
    ThreadArguments* thread_args = (ThreadArguments*) arguments;
    int my_rank = thread_args->my_rank;
    int bin_count = thread_args->bin_count;
    int data_count = thread_args->data_count;
    int* array = thread_args->array;
    double* bin_limits = thread_args->bin_limits;
    int* histogram = thread_args-> histogram;
    pthread_mutex_t* mutex = thread_args->mutex;

    int* local_histogram = new int [bin_count]();
    int el_per_thread = data_count/thread_count;
    int first_idx = my_rank*el_per_thread;
    int last_idx = first_idx+el_per_thread;

    // Computing local histogram per thread
    for (int i =first_idx; i<last_idx; i++){
        for (int j=1; j<bin_count+1;j++){
            if (array[i]<bin_limits[j]){
                local_histogram[j-1]++;
                break;
            }
        }
    }
    // Updating global histogram with mutex lock
    pthread_mutex_lock(mutex);
    for (int i=0; i<bin_count; i++){
        histogram[i]+=local_histogram[i];
    }
    pthread_mutex_unlock(mutex);
    delete[] local_histogram;
    return NULL;
}