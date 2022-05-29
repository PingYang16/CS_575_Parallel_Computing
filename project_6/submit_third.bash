#!/bin/bash
#SBATCH -J third
#SBATCH -A cs475-575
#SBATCH -p class
#SBATCH --constraint=v100
#SBATCH --gres=gpu:1
#SBATCH -o third.out
#SBATCH -e third.err
#SBATCH --mail-type=BEGIN,END,FAIL
#SBATCH --mail-user=yangpin@oregonstate.edu

for n in 1024 2*1024 4*1024 8*1024 16*1024 32*1024 64*1024 128*1024 256*1024 512*1024 1024*1024 2*1024*1024 4*1024*1024 8*1024*1024
do
    for s in 32 64 128 256
    do
        g++ -DNUM_ELEMENTS=$n -DLOCAL_SIZE=$s -o third third.cpp /usr/local/apps/cuda/10.1/lib64/libOpenCL.so.1.1 -lm -fopenmp 
        ./third
    done
done