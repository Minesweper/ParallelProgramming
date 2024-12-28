#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


int main(int argc, char *argv[]){
    int lo,num,rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &num);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Status status;
    int ans = rank + 1, tmp = 0;
    
    for (int i = 1; i <= lo; i++){
        int tag = i;
        int step = (int)pow(2, i);
        if (pid % step == 0)
        {
            MPI_Recv(&tmp, 1, MPI_INT, pid + step/2, tag, MPI_COMM_WORLD, &status);
            ans += tmp;
        }
        else if(pid % step == step/2) MPI_Send(&ans, 1, MPI_INT, pid - step/2, tag, MPI_COMM_WORLD);
        
    }
    for (int i = lo; i > 0; i--){
        int tag = i;
        int step = (int)pow(2, i);
        if (pid % step == 0) MPI_Send(&ans, 1, MPI_INT, pid + step / 2, tag, MPI_COMM_WORLD);
        
        else if(pid % step == step/2){
            MPI_Recv(&tmp, 1, MPI_INT, pid - step / 2, tag, MPI_COMM_WORLD, &status);
            ans = tmp;
        }
    }

    if(rank == 0) {
        printf("Tree-level sum:\n");
    }
    MPI_Barrier(MPI_COMM_WORLD);
    printf("id = %d, sum = %d\n", rank, data);
    MPI_Finalize();
    return 0;
}