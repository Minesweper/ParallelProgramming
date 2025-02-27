#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

void print_mat(int *a, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%.2d ",a[i*n + j ]);
        }
        printf("\n");
    }
}

void matrix_multiply(int *a, int *b, int *c, int n) {
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            for (int k = 0; k < n; k++) {
                c[i * n + j] += a[i * n + k] * b[k * n + j];
            }
        }
    }
}
void get_block(int *src, int *block, int i_th, int j_th, int srcsize, int blocksize) {
    for (int i = 0; i < blocksize; i++) {
        for (int j = 0; j < blocksize; j++) {
            block[i * blocksize + j] = src[(i_th * blocksize + i) *srcsize + j_th * blocksize + j];
        }
    }
}
void put_block(int *dest, int *block, int i_th, int j_th, int destsize, int blocksize) {
    for (int i = 0; i < blocksize; i++) {
        for (int j = 0; j < blocksize; j++) {
            dest[(i_th * blocksize + i) *destsize + j_th * blocksize + j] = block[i * blocksize + j];
        }
    }
}
void fox(int *a, int *b, int *c, int n, int p, MPI_Comm comm) {
    int sqrt_p = (int)sqrt(p);
    int block_size = n / sqrt_p;
    if(block_size * sqrt_p != n) {
        printf("please check n and p!\n");
        return;
    }
    int rank, size;
    MPI_Comm_rank(comm, &rank);
    MPI_Comm_size(comm, &size);

    int *a_block = (int *)malloc(block_size * block_size * sizeof(int));
    int *b_block = (int *)malloc(block_size * block_size * sizeof(int));
    int *c_block = (int *)malloc(block_size * block_size * sizeof(int));
    int *temp_block = (int *)malloc(block_size * block_size * sizeof(int));

    for (int i = 0; i < block_size * block_size; i++) {
        c_block[i] = 0;
    }

    int pi = rank / sqrt_p;
    int pj = rank % sqrt_p; 
   
    if (rank == 0) {
        
        for (int i = 0; i < sqrt_p; i++) {
            for (int j = 0; j < sqrt_p; j++) {
                if (i == 0 && j == 0) {
                    get_block(a, a_block, pi, pj, n, block_size);
                    get_block(b, b_block, pi, pj, n, block_size);
                    continue;
                }
                get_block(a, temp_block, i, j, n, block_size);
                MPI_Send(temp_block, block_size * block_size, MPI_INT, i * sqrt_p + j, 0, MPI_COMM_WORLD);
                get_block(b, temp_block, i, j, n, block_size);
                MPI_Send(temp_block, block_size * block_size, MPI_INT, i * sqrt_p + j, 1, MPI_COMM_WORLD);
            }
        }
    } else {
        
        MPI_Recv(a_block, block_size * block_size, MPI_INT, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        MPI_Recv(b_block, block_size * block_size, MPI_INT, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    }
    
    MPI_Comm row_comm; 
    MPI_Comm_split(comm, pi, rank, &row_comm);

    for (int k = 0; k < sqrt_p; k++) {
        int root = (pi + k) % sqrt_p;
        if (pj == root) {
            
            for (int i = 0; i < block_size*block_size; i++) {
                temp_block[i] = a_block[i];
            }
        }
    
        MPI_Bcast(temp_block,block_size*block_size, MPI_INT, root, row_comm);
        matrix_multiply(temp_block, b_block, c_block, block_size);
        
        int sendtoi = (pi - 1 + sqrt_p) % sqrt_p;
        int recvfromi = (pi + 1) % sqrt_p;
        MPI_Sendrecv(b_block, block_size*block_size, MPI_INT, sendtoi * sqrt_p + pj, 1, 
                    temp_block, block_size*block_size, MPI_INT, recvfromi * sqrt_p + pj, 1, 
                    MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        for (int i = 0; i < block_size * block_size; i ++) {
            b_block[i] = temp_block[i];
        }
    }
    
    if (rank != 0) {
      
        MPI_Send(c_block, block_size*block_size, MPI_INT, 0, 3, MPI_COMM_WORLD);
    } else {
        
        for (int i = 0; i < sqrt_p; i++) {
            for (int j = 0; j < sqrt_p; j++) {
                if(i == 0 && j == 0) {
                    put_block(c, c_block, i, j, n, block_size);
                } else {
                    MPI_Recv(temp_block, block_size*block_size, MPI_INT, i * sqrt_p + j, 3, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
                    put_block(c, temp_block, i, j, n, block_size);
                }
            }
        }
    }
    
    free(a_block);
    free(b_block);
    free(c_block);
    free(temp_block);
}

int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int rank, size;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &size);

    int n = 4;
    int p = size;

    int *a = NULL;
    int *b = NULL;
    int *c = NULL;
    if (rank == 0) {
        a = (int *)malloc(n * n *sizeof(int));
        b = (int *)malloc(n * n *sizeof(int));
        c = (int *)malloc(n * n *sizeof(int));
        for (int i = 0; i < n * n; i++) {
            a[i] = i + 1;
            b[i] = i + 1;
        }
    }
    fox(a, b, c, n, p, MPI_COMM_WORLD);
    if (rank == 0) {
        print_mat(a, n);
        print_mat(b, n);
        print_mat(c, n);
        free(a);
        free(b);
        free(c);
    }
    MPI_Finalize();
    return 0;
}
