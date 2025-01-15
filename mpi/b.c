#include "mpi.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void alltoall(int* send, int count1, MPI_Datatype sendtype, int* recv, int count2, MPI_Datatype recvtype, MPI_Comm comm){
    int rank, size;
    MPI_Status status;
    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &size);
    for (int i = 0; i < size; i++){
        if (i != rank){
            MPI_Send(send + i * count1, count1, sendtype, i, rank , comm);
            MPI_Recv(recv + i * count2, count2, recvtype, i, i, comm, &status);
        }
        else recv[i] = send[i];
        
    }
}

int main(int argc, char* argv[])
{
    int i, rank, size, tag = 1;
    int *senddata;
    int *recvdata;
    double star, en, st, et;
    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);
    senddata = (int*)malloc(size * sizeof(int));
    recvdata = (int*)malloc(size * sizeof(int));
    for (int j = 0; j < size; j++) senddata[j] = j;
    
    star = MPI_Wtime();
    alltoall(senddata, 1, MPI_INT, recvdata, 1, MPI_INT, MPI_COMM_WORLD);
    en = MPI_Wtime();
    MPI_Reduce(&star, &st, 1, MPI_DOUBLE, MPI_MIN, 0, MPI_COMM_WORLD);
    MPI_Reduce(&en, &et, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
	for (i = 0; i < size; i++)
		printf("My rank =  %d  After myalltoall recv = %d\n", rank, recvdata[i]);
    if (rank == 0) printf("total time = %f\n", et - st);

    MPI_Barrier(MPI_COMM_WORLD);

    star = MPI_Wtime();
    MPI_Alltoall(senddata, 1, MPI_INT, recvdata, 1, MPI_INT, MPI_COMM_WORLD);
    en = MPI_Wtime();
    MPI_Reduce(&star, &st, 1, MPI_DOUBLE, MPI_MIN, 0, MPI_COMM_WORLD);
    MPI_Reduce(&en, &et, 1, MPI_DOUBLE, MPI_MAX, 0, MPI_COMM_WORLD);
	for (i = 0; i < size; i++)
		printf("My rank =  %d  recv = %d\n", rank, recvdata[i]);

    if (rank == 0) printf("total time = %f\n", et - st);
    MPI_Finalize();
    return 0;
}
