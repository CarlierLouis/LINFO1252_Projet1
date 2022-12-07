#!/bin/bash

echo "Mesure, 1 thread, 2 threads, 4 threads, 8 threads, 16 threads, 32 threads, 64 threads" > task1/data/task1_1.csv
echo "Mesure, 1 thread, 2 threads, 4 threads, 8 threads, 16 threads, 32 threads" > task1/data/task1_2.csv
echo "Mesure, 1 thread, 2 threads, 4 threads, 8 threads, 16 threads, 32 threads" > task1/data/task1_3.csv

gcc -o task1/task1_1 -lpthread -Wall -Werror task1/task1_1.c
gcc -o task1/task1_2 -lpthread -Wall -Werror task1/task1_2.c
gcc -o task1/task1_3 -lpthread -Wall -Werror task1/task1_3.c

DATE='/bin/date'

for ((i=1;i<=5;i++))
do
    declare -a philoArray
    declare -a consoproduArray
    declare -a readerwriterArray

    for ((j=0;j<=6;j++))
    do
        exp=$((2**$j))
        arg=$(($exp*2))

        startTime=$($DATE +'%s%N')
    
        ./task1/task1_1 $arg

        endTime=$($DATE +'%s%N')

        totalTime=$(($endTime-$startTime))

        philoArray+=($totalTime)
    done

    for ((j=0;j<=5;j++))
    do
        exp=$((2**$j))

        startTime=$($DATE +'%s%N')

        ./task1/task1_2 $exp $exp

        endTime=$($DATE +'%s%N')

        totalTime=$(($endTime-$startTime))

        consoproduArray+=($totalTime)
    done    

    for ((j=0;j<=5;j++))
    do
        exp=$((2**$j))

        startTime=$($DATE +'%s%N')

        ./task1/task1_3 $exp $exp

        endTime=$($DATE +'%s%N')

        totalTime=$(($endTime-$startTime))

        readerwriterArray+=($totalTime)
    done    

    echo $i,${philoArray[*]} | tr ' ' ',' >> task1/data/task1_1.csv
    unset philoArray

    echo $i,${consoproduArray[*]} | tr ' ' ',' >> task1/data/task1_2.csv
    unset consoproduArray

    echo $i,${readerwriterArray[*]} | tr ' ' ',' >> task1/data/task1_3.csv
    unset readerwriterArray

done

echo 'philo :'
cat task1/data/task1_1.csv

echo 'consoprodu :'
cat task1/data/task1_2.csv

echo 'readerwriter :'
cat task1/data/task1_3.csv
