## USTC 2024 Parallel Programming (COMP6201P.01) Course Lab

### MPI Usage

<strong>Linux:</strong>

```
sudo apt-get install mpich

mpicc -lm -o <EXAMPLE> <EXAMPLE.c>
mpiexec -n <CORE_NUM> <./EXAMPLE>
```

In ```mpi/```, to be continued.

### CUDA Usage

<strong>Use ```nvcc -V``` to check environment.</strong>

```
./run.sh
```

In ```cuda/```, original source code named as ```bench.cu```.
