#!/bin/bash
#SBATCH -J project7
#SBATCH -A cs475-575
#SBATCH -p class
#SBATCH -N 12 # number of nodes
#SBATCH -n 12 # number of tasks
#SBATCH --constraint=ib
#SBATCH -o project7.out
#SBATCH -e project7.err
#SBATCH --mail-type=END,FAIL
#SBATCH --mail-user=yangpin@oregonstate.edu 
module load openmpi
mpic++ project7.cpp -o project7 -lm
for p in 1 2 3 4 5 6 7 8 9 10 11 12
do
	mpiexec -mca btl self,tcp -np $p ./project7
done
