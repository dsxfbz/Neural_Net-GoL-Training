/*
 * gen_seed.cpp
 * 
 * Copyright 2022 Daniel Xiang <daniel@debian-11-school>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */


#include <iostream>

#include "gol.h"
#include "neural_net.h"

int main(int argc, char **argv)
{
	srand(clock());
	
	FILE * seed;
	int num_weights = 4096*3;
	
	float * test = new float[num_weights];
	
	float weights[num_weights];
	weights[0] = float(1)/4096;
	
	std::string filename;
	
	for (int i=0; i<16; i++) {
		for (int k=1; k<num_weights; k++) {
			if (rand()%2>0) {
				weights[k] = weights[k-1]+(float(1)/((rand()%4096)+1));
			}
			else {
				weights[k] = weights[k-1]-(float(1)/((rand()%4096)+1));
			}
		}
		filename = "curr_gen/"+std::to_string(i)+".dat";
		std::cout<<filename<<"\n";
		
		seed = std::fopen(filename.c_str(), "w");
		std::fwrite(weights, sizeof(float), 1, seed);
	}
	
	delete[] test;
	return 0;
}

