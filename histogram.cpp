#include <cstdlib>
#include <cstdio>
#include <pthread.h>
#include <cmath>
#include <iostream>

// A struct to pass arguments to per thread function (for cleaner code purpose)
struct ThreadArguments {
    int my_rank;
    int bin_count;
    int data_count;
    int* array;
    double* bin_limits;
    int* global_histogram;
    int* tree_struct_histogram;
    int** local_histogram;
    pthread_mutex_t* mutex;
    int* divisors;
    int divisor_length;
};

// Predefining the functions to be defined 
double* get_bin_limits(int min, int max, int bin_count);
void*  bin_per_thread(void* arguments);

// Creating a global variables for total thread number and barrier
int thread_count ;
pthread_barrier_t barrier ;

int main(int argn, char* argv[]){
    if (argn!=6){ // Error handling for wrong inputs
        std::cerr<< "Error: expected 5 arguments"<<std::endl;
    }
    // Extracting command line arguments (thread_count, bin_count, min_value, max_value and length of the array)
    thread_count = std::stoi(argv[1]);
    const int bin_count = std::stoi(argv[2]);
    int min_val = std::stoi(argv[3]);
    int max_val = std::stoi(argv[4]);
    const int data_count  = std::stoi(argv[5]);
    // Set seed for the random number generator
    
    // Divisor for the tree sum
    int divisor_length = static_cast<int>(std::log2(thread_count));
    int divisors[divisor_length];
    for (int k=0; k<divisor_length; k++) divisors[k] = std::pow(2,k+1);
    
    // Create measurement array and populate the array with random numbers with in the given range
    srand(100);
    int array [data_count];
    for (int j=0; j<data_count;j++) array[j]=rand()%(max_val-min_val)+min_val;

    // Initialize the local_sum, global_sum and tree structured sum histograms (to be passed to each thread)
    int* global_histogram =  new int[bin_count]();
    int* tree_struct_histogram= new int[bin_count]();
    int** local_histogram= new int* [thread_count];
    for (int j=0; j< thread_count; j++) local_histogram[j]= new int [bin_count]{};
    
    // Other important variable definitions
    long thread;    
    double* bin_limits = get_bin_limits(min_val, max_val,bin_count);
    // Define the mutex, thread_handles and initialize the barrier
    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, NULL);
    pthread_barrier_init(&barrier, NULL, thread_count);
    pthread_t* thread_handles;
    thread_handles = (pthread_t*)malloc(thread_count*sizeof(pthread_t));
    // Create an array of struct (for multiple threads)
    ThreadArguments* thread_args = new ThreadArguments[thread_count];

    // SPAWNING multiple threads (# of thread_count)
    // Easier to use struct to pass data for each thread (more organized)
    for (thread=0; thread<thread_count; thread++){
        // Fill in the per thread arguments for the struct
        thread_args[thread].my_rank = thread;
        thread_args[thread].bin_count = bin_count;
        thread_args[thread].data_count = data_count;
        thread_args[thread].array = array;
        thread_args[thread].bin_limits = bin_limits;
        thread_args[thread].global_histogram = global_histogram;
        thread_args[thread].local_histogram = local_histogram;
        thread_args[thread].tree_struct_histogram = tree_struct_histogram;
        thread_args[thread].divisors = divisors;
        thread_args[thread].divisor_length=divisor_length;
        thread_args[thread].mutex = &mutex;
        // Create and run processes in threads
        pthread_create(&thread_handles[thread], NULL, bin_per_thread,(void* )(&thread_args[thread]));
    }

    // Joint the threads
    for (thread=0; thread<thread_count; thread++){
        pthread_join(thread_handles[thread], NULL);
    }
    // Print results for the global sum
    std::cout<<"Global Sum"<< std::endl;
    std::cout<<"bin_maxes: ";
    for (int j=0; j<bin_count;j++){
        std::cout<< bin_limits[j+1]<<" ";
    }
    std::cout<<std::endl;
    std::cout<<"bin_counts: ";
    
    for (int j=0; j<bin_count;j++){
        std::cout<< global_histogram[j]<<" ";
    }
    std::cout<<std::endl;
    
    // Print results for the tree structured sum
    std::cout<<"\nTree structured Sum"<< std::endl;
    std::cout<<"bin_maxes: ";
    for (int j=0; j<bin_count;j++){
        std::cout<< bin_limits[j+1]<<" ";
    }
    std::cout<<std::endl;
    std::cout<<"bin_counts: ";
    for (int j=0; j<bin_count;j++){
        std::cout<< tree_struct_histogram[j]<<" ";
    }
    std::cout<<std::endl;


    // Clean up before exiting
    for (int j=0; j<thread_count; j++){
        delete[] local_histogram[j];
    }
    delete[] local_histogram;

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
    int* global_histogram = thread_args-> global_histogram;
    int** local_histogram = thread_args-> local_histogram;
    int divisor_length= thread_args->divisor_length;
    int* tree_struct_histogram = thread_args-> tree_struct_histogram;
    int* divisors = thread_args->divisors;
    pthread_mutex_t* mutex = thread_args->mutex;

    // int* local_histogram = new int [bin_count]();
    int el_per_thread = data_count/thread_count;
    int first_idx = my_rank*el_per_thread;
    int last_idx = first_idx+el_per_thread;

    // Computing local histogram per thread
    for (int i =first_idx; i<last_idx; i++){
        for (int j=1; j<bin_count+1;j++){
            if (array[i]<bin_limits[j]){
                local_histogram[my_rank][j-1]++;
                break;
            }
        }
    }
    // Updating global histogram with mutex lock
    pthread_mutex_lock(mutex);
    for (int i=0; i<bin_count; i++){
        global_histogram[i]+=local_histogram[my_rank][i];
    }
    pthread_mutex_unlock(mutex);

    // Updating the local_histogram using tree structured sum 
    pthread_barrier_wait(&barrier);
    for (int i=0; i< divisor_length; i++){
        int divisor = divisors[i];
        int core_dce = std::pow(2,i);
        if (my_rank%divisor==0){
            for (int j=0; j<bin_count; j++){
                local_histogram[my_rank][j]+= local_histogram[my_rank+core_dce][j];
            }
        }
        pthread_barrier_wait(&barrier); // Barrier to avoid the fastest threads going ahead w/o each level is completed by all threads
    }
    // Copy the tree structured sum
    if (my_rank==0){
        for (int i=0; i<bin_count; i++){
            tree_struct_histogram[i]=local_histogram[0][i];
        }
    }
    return NULL;
}