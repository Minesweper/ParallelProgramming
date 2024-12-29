#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>


// binary tree sum all nodes
int main(int argc, char *argv[]){
    int size,rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Status status;

    int ans = rank + 1, num = size, tmp = 0;
    for (num; num > 1; num /= 2) {
        if (rank < num / 2) {            
            MPI_Recv(&tmp, 1, MPI_INT, rank + num / 2, 0, MPI_COMM_WORLD, &status);
            ans += tmp; 
        } 
        else {
            int dest = rank - num / 2;
            MPI_Send(&ans, 1, MPI_INT, dest, 0, MPI_COMM_WORLD);
            break;
        }
    }

    for (num = 1; num < size; num *= 2) {
        if (rank < num) {            
            MPI_Send(&ans, 1, MPI_INT, rank + num, 0, MPI_COMM_WORLD);           
        } else if(rank < num * 2 ) {           
            int source = rank - num;
            MPI_Recv(&ans, 1, MPI_INT, source, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        }
    }

    if(rank == 0) {
        printf("Tree-level sum:\n");
    }
    MPI_Barrier(MPI_COMM_WORLD);
    printf("id = %d, sum = %d\n", rank, ans);
    MPI_Finalize();
    return 0;
}
