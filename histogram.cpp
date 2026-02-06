#include <cstdlib>
#include <cstdio>
#include <pthread.h>

int thread_count ;
double* get_bin_limits(int min, int max, int bin_count);
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

    for (int i = 0; i<array_size; i++){
        for (int j=1; j<bin_count+1;j++){
            if (array[i]<bin_limits[j]){
                histogram[j-1]++;
                break;
            }
        }
    }
    for (int j=0; j<bin_count;j++){
        printf("%d\n", histogram[j]);
    }


    delete[] bin_limits;
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