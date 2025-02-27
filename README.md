## USTC 2024 Parallel Programming (COMP6201P.01) Course Lab

### MPI

<strong>Linux:</strong>

```
sudo apt-get install mpich

mpic++ -lm -o <EXAMPLE> <EXAMPLE.c>
mpiexec -n <CORE_NUM> <./EXAMPLE>

example:

mpic++ -lm -o lab a.c
mpiexec -n 16 ./lab
```

In ```mpi/```, to be continued.

### CUDA

<strong>Use ```nvcc -V``` to check your environment.</strong>

```
./run.sh
```

In ```cuda/```, original source code named as ```bench.cu```.
