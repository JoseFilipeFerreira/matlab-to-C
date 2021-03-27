#!/bin/sh

#PBS -N multi_room_distribution
#PBS -l walltime=2:00:00
#PBS -l nodes=1:r662:ppn=48
#PBS -q mei

module load gcc/7.2.0

cd $PBS_O_WORKDIR

for n_students in 100 200 300 500 1000; do
    for maxi in {1..3}; do
        for tmin in 0.1 0.01 0.001 0.0001 0.00001; do
            for cooldown in 0.9999 0.999 0.99 0.9 0.8; do
                echo "running: ./rooms $n_students 1000 $maxi""00 $tmin $cooldown"
                ./rooms "$n_students" 1000 "$maxi"00 "$tmin" "$cooldown" &> "$n_students"_"$maxi"_"$tmin"_"$cooldown".output
            done
        done
    done
done
