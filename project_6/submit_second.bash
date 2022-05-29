#!/bin/bash
#SBATCH -J second
#SBATCH -A cs475-575
#SBATCH -p class
#SBATCH --constraint=v100
#SBATCH --gres=gpu:1
#SBATCH -o second.out
#SBATCH -e second.err
#SBATCH --mail-type=BEGIN,END,FAIL
#SBATCH --mail-user=yangpin@oregonstate.edu

for n in 1024 2*1024 4*1024 8*1024 16*1024 32*1024 64*1024 128*1024 256*1024 512*1024 1024*1024 2*1024*1024 4*1024*1024 8*1024*1024
do
    for s in 8 16 32 64 128 256 512
    do
        g++ -DNUM_ELEMENTS=$n -DLOCAL_SIZE=$s -o second second.cpp /usr/local/apps/cuda/10.1/lib64/libOpenCL.so.1.1 -lm -fopenmp 
        ./second
    done
done