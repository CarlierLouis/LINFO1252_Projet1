#!/bin/bash

echo "Mesure, 1 thread, 2 threads, 4 threads, 8 threads, 16 threads, 32 threads, 64 threads" > data/task1_1.csv
echo "Mesure, 1 thread, 2 threads, 4 threads, 8 threads, 16 threads, 32 threads" > data/task1_2.csv
echo "Mesure, 1 thread, 2 threads, 4 threads, 8 threads, 16 threads, 32 threads, 64 threads" > data/task1_3.csv

gcc -o task1_1 task1_1.c
gcc -o task1_2 task1_2.c
#gcc -o task1_3 task1_3.c

for ((i=1;i<=5;i++))
do
    declare -a philoArray
    declare -a consoproduArray
    declare -a philoArray
    
    for ((j=0;j<=6;j++))
    do
        exp=$((2**$j))
        arg=$((exp*2))

        time=$(/usr/bin/time -f %e ./task1_1 $arg 2>&1)

        philoArray+=($time)
    done

    for ((j=0;j<=5;j++))
    do
        exp=$((2**$j))
        arg=$((exp*2))

        time=$(/usr/bin/time -f %e ./task1_2 $arg $arg 2>&1)

        consoproduArray+=($time)
    done    

    echo $i,${philoArray[*]} | tr ' ' ',' >> data/task1_1.csv
    unset philoArray

    echo $i,${consoproduArray[*]} | tr ' ' ',' >> data/task1_2.csv
    unset consoproduArray

done
