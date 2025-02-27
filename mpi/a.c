#include<stdio.h>
#include<mpi.h>
#include<stdlib.h>
#include<string.h>

int main(int argc, char** argv) {
    int rank, size;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int group_num = 2;
    int color, key;
    color = rank % group_num;
    key = rank / group_num;
    MPI_Comm new_group;
    MPI_Comm_split(MPI_COMM_WORLD, color, key, &new_group);

    int new_rank, new_size;
    MPI_Comm_rank(new_group, &new_rank);
    MPI_Comm_size(new_group, &new_size);

    int message = 0;
    int cast_root = 2;
    
    if(rank == cast_root) {
        message = 888;
        printf("Cast root rank: %d\n",cast_root);
        for(int i = 0; i < size; i++) {
            if( (i != cast_root) && i / group_num == 0) {
                MPI_Send(&message, 1, MPI_INT, i, 0, MPI_COMM_WORLD);
            }
        }
    } else {
        if(rank / group_num == 0) {
            MPI_Recv(&message, 1, MPI_INT, cast_root, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            printf("rank %d, receive from rank %d\n",rank,cast_root);
        }
    }

    MPI_Bcast(&message, 1, MPI_INT, 0, new_group);

    printf("Global rank: %d\n", rank);
    MPI_Comm_free(&new_group);
    MPI_Finalize();
    return 0;
}
