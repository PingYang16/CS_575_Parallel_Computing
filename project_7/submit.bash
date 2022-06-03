#!/bin/bash
#SBATCH -J project7
#SBATCH -A cs475-575
#SBATCH -p class
#SBATCH -N 16 # number of nodes
#SBATCH -n 16 # number of tasks
#SBATCH --constraint=ib
#SBATCH -o project7.out
#SBATCH -e project7.err
#SBATCH --mail-type=END,FAIL
#SBATCH --mail-user=yangpin@cs.oregonstate.edu 
module load openmpi
mpic++ project7.cpp -o project7 -lm
mpiexec -mca btl self,tcp -np 16 ./project7
