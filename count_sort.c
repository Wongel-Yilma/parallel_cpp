#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _OPENMP
#include <omp.h>
#endif

void count_sort(int* , int); 
int thread_count; // Defining a global variable so that it can be accessed from anywhere

int main(int argn, char * argv[]){
    thread_count = strtol(argv[1],NULL,10); // Getting the thread count from command line arg
    int n = strtol(argv[2],NULL,10);// Getting the number of elements from command line arg
    srand(100); // Setting random seed
    int * arr = malloc(n*sizeof(int)); // Creating a variable for a new arr
    printf("Original array: ");
    for (int i=0; i<n; i++){  // Generating random integers from 1 to 100 and printing them
        arr[i]=rand()%n +1;
        printf("%d ", arr[i]);
    }
    printf("\n");
    count_sort(arr, n); // Calling the count_sort function
    printf("Sorted array: ");
    for (int i=0; i<n; i++){ 
        printf("%d ", arr[i]); // Printing the sorted values
    }
    free(arr);
    return 0;
}

void count_sort(int a [], int n){
    int i, j, count;
    int *temp = malloc(n*sizeof(int)); // Create a temporaty variable 
    #pragma omp parallel num_threads(thread_count) private(i,j, count) shared (temp,a, n) // Spawn multiple threads using openmp
    {    
        #pragma omp for     // Declaring th for loop in the next line to be handled by openmp
            for(i=0; i<n; i++){  // Openmp will divide the loop over the number of threads internally
                count=0;          // Initializing the value of count to 0 (for each thread)
                for(j =0; j<n;j++){
                    if (a[i]>a[j]) count++; 
                    else if(a[i]==a[j]&& i>j) count++;  // Get the sorted position of the ith element
                }
                temp[count] =a[i]; // Using the sorted position (count), copy the value from array "a" to a "temp" array
            }
        #pragma omp for // copy the values from the temp array to the original array "a" in parallel using omp
        for(i=0; i<n; i++){ 
            a[i] = temp[i]; 
        }
    }    
// memcpy(a, temp , n*sizeof(int));  // This is serial implementation.

    free(temp);
}
