#!/bin/sh

#PBS -N AP
#PBS -l walltime=2:00:00
#PBS -l nodes=1
#PBS -q cpar

module load gcc/5.3.0

cd $PBS_O_WORKDIR

# for run in {1..8}; do
    ./main
# done
