#!/bin/bash
clear
g++ -g mutate.cpp -o mutate.exe
for ((k=0; k<4; k++)); do
	echo ${k}
	valgrind --leak-check=full ./mutate.exe ${k}
	#./mutate.exe ${k}
done
