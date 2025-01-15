#!/bin/bash

N=256
T=8

python3 RandGen.py $N
nvcc -lm life3d.cu -o life3d
nvcc -lm bench.cu -o bench
./life3d $N $T data/data.in data/data.out
./bench $N $T data/data.in data/data.out

rm life3d bench
