#!/bin/bash
clear

num_gen=1
echo "Number of generations: ${num_gen}"

num_cores=$(lscpu -b -p=Core,Socket | grep -v '^#' | sort -u | wc -l)

#Run saved neural nets fron curr_gen, create new ones in new_gen from most successful
g++ run_gen_part.cpp -o run_gen_part.exe -O3
g++ mutate.cpp -o mutate.exe -O3

#g++ -g run_gen_part.cpp -o run_gen_part.exe
#g++ -g mutate.cpp -o mutate.exe


for ((i=0; i<${num_gen}; i++)); do
	echo "generation ${k}"
	
	#Test generation
	for ((k=0; k<16; k++)); do
		for ((l=k+1; l<16; l++)); do
			echo "running game ${i}-${k}-${l}"
			#valgrind --leak-check=full --track-origins=yes ./run_gen_part.exe "${k}" "${l}"
			./run_gen_part.exe "${k}" "${l}"
		done
	done
	
	echo "Creating new neural nets"
	for ((k=0; k<4; k++)); do
		echo ${k}
		#valgrind --leak-check=full ./mutate.exe ${k}
		./mutate.exe ${k}
	done
	rm results.dat
	
	echo "Moving Files"
	rm curr_gen/*
	mv new_gen/* curr_gen
done


