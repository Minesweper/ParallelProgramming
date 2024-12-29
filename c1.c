#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

int main(int argc, char *argv[]){
    int num, rank;
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &num);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Status status;
    int ans = rank + 1;
    int tmp = 0;
    int lo = (int)log2(num);
    for(int i = 0; i < lo; i++) {
        int tag = i+1;
        int step = (int)pow(2,i);
        int dest = rank ^ step;
        MPI_Send(&ans, 1, MPI_INT, dest, tag,  MPI_COMM_WORLD);
        MPI_Recv(&tmp, 1, MPI_INT, dest, tag,  MPI_COMM_WORLD, &status);
        ans += tmp;
    }
    if(rank == 0) {
        printf("Butterfly-level sum:\n");
    }
    MPI_Barrier(MPI_COMM_WORLD);
    printf("id = %d, sum = %d\n", rank, ans);
    MPI_Finalize();

    return 0;
}
