#!/bin/bash
DIR=$(dirname "$0")
cd $DIR
gcc Brownian.c -lm -o brownian.out
./brownian.out
gnuplot script.gnuplot -p
sleep 2
#rm Path.dat
rm brownian.out
exit
