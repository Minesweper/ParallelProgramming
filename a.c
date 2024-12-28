#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    int rank, num;
    char tmp[16];
    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &num);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    if(rank == 0) {
        strncpy(tmp, "Hello World", 16);
    }
    MPI_Comm world;
    MPI_Comm_split(MPI_COMM_WORLD, rank % 4, rank, &world);
    int rank1,num1;
    MPI_Comm_rank(world, &rank1);
    MPI_Comm_size(world, &num1);
    MPI_Group world_group, group;
    int fir[] = {0, 1, 2, 3};
    MPI_Comm_group(MPI_COMM_WORLD, &world_group);
    MPI_Group_incl(world_group, 4, fir, &group);

    MPI_Comm firstworld;
    MPI_Comm_create(MPI_COMM_WORLD, group, &firstworld);

    MPI_Barrier(MPI_COMM_WORLD);
    MPI_Bcast(tmp, 16, MPI_CHAR, 0, firstworld);
    MPI_Barrier(MPI_COMM_WORLD);

    MPI_Bcast(tmp, 16, MPI_CHAR, 0, world);
    MPI_Barrier(MPI_COMM_WORLD);

    printf("rank %d, num %d, new num %d, tmp %s\n", rank, num, num1, tmp);
    MPI_Finalize();
    return 0;
}