#!/bin/bash
#PBS -S /bin/sh
#PBS -N hw5
#PBS -A eecs587f16_flux
#PBS -l qos=flux
#PBS -l procs=16,walltime=0:08:00
#PBS -l pmem=4000mb
#PBS -q flux
#PBS -m abe
#PBS -j oe
#PBS -V
echo "2000 16 processors"
#cat $PBS_NODEFILE
# Let PBS handle your outpu
cd /home/binbinli/EECS587project
mpic++ -std=c++11 -o main reversi.cpp treeNode.cpp main.cpp
mpirun -np 16 ./main
