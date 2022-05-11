#!/bin/bash

for n in 1000 2000 4000 8000 16000 32000 64000 128000 256000 512000 1024000 2048000 4096000 8192000
do
	g++ -DARRAYSIZE=$n project4.cpp -o project4 -lm -fopenmp
	./project4
done
