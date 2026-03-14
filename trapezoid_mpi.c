#include <stdio.h>
#include <string.h>

#include <mpi.h>


double f(double);
double Trapz(int , double , double, double );
void Get_input(int , int , double* , double* , int* );
int main(){
    int my_rank, comm_sz, n,local_n;
    double a, b, h, local_a, local_b;
    double local_int, global_int;
    int q;
    MPI_Init(NULL, NULL);
    MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
    MPI_Comm_size(MPI_COMM_WORLD,&comm_sz );
    Get_input(my_rank, comm_sz, &a, &b, &n);
    h = (b-a)/n;
    local_n = n/comm_sz;
    local_a = a+local_n*h*my_rank;
    local_b = local_a+h*local_n;
    local_int = Trapz( local_n,  local_a,  local_b,  h);

    if (my_rank!=0){
        MPI_Send(&local_int, 1, MPI_DOUBLE, 0, 0, MPI_COMM_WORLD);
    }
    else if (my_rank==0){
        global_int = local_int;
        for (int q=1; q<comm_sz;q++){
            MPI_Recv(&local_int, 1, MPI_DOUBLE, q, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            global_int+=local_int;
        }
        printf("Total area: %f\n", global_int);
    }
    MPI_Finalize();
    return 0;
}

double f(double x){
    return x*x+2;
}

double Trapz(int local_n, double local_a, double local_b, double h){
    double local_sum = (f(local_a)+f(local_b))/2;
    for (int i=1; i<local_n; i++){
        local_sum += f(local_a+h*i);
    }
    local_sum*=h;
    return local_sum;
}
void Get_input(int my_rank, int comm_sz, double* a_p, double* b_p, int* n_p){
    int dest;

    if (my_rank==0){
        printf("Provide a, b and n.\n");
        scanf("%1lf %1lf %1d", a_p, b_p, n_p);
        for (dest=1; dest<comm_sz; dest++){
            MPI_Send(a_p, 1, MPI_DOUBLE, dest,0, MPI_COMM_WORLD);
            MPI_Send(b_p, 1, MPI_DOUBLE, dest,0, MPI_COMM_WORLD);
            MPI_Send(n_p, 1, MPI_INT, dest,0, MPI_COMM_WORLD);
        }
    }
    else{
        MPI_Recv(a_p, 1, MPI_DOUBLE, 0 ,0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(b_p, 1, MPI_DOUBLE, 0 ,0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(n_p, 1, MPI_INT, 0 ,0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
}



