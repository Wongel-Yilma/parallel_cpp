#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _OPENMP
#include <omp.h>
#endif
void count_sort(int* , int);

int main(int argn, char * argv[]){
    // int n = strtol(argv[1],NULL,10);
    int n = 20;
    srand(100);
    int * arr = malloc(n*sizeof(int));
    printf("Original array: ");
    for (int i=0; i<n; i++){ 
        arr[i]=rand()%100;
        printf("%d ", arr[i]);
    }
    printf("\n");
    count_sort(arr, n);
    printf("Sorted array: ");
    for (int i=0; i<n; i++){ 
        printf("%d ", arr[i]);
    }
    free(arr);
    return 0;
}

void count_sort(int a [], int n){
    int i, j, count;
    int *temp = malloc(n*sizeof(int));
    for(i=0; i<n; i++){
        count=0;
        for(j =0; j<n;j++){
            if (a[i]>a[j]) count++;
            else if(a[i]==a[j]&& i>j) count++;
        }
        temp[count] =a[i];
    }

    memcpy(a, temp , n*sizeof(int));
    free(temp);
}