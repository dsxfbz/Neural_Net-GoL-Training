/*
 * mutate.cpp
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

#include "neural_net.h"
#include "gol.h"

#include <iostream>
#include <typeinfo>


void select(int win_count[16], int selected[4]);
int smallest(int win_count[16], int selected[4]);


int main(int argc, char **argv)
{
	//GoL game;
	FILE * wl_data;
	int buffer[1];
	
	int win_count[16];
	int selected[4];
	for (int i=0; i<16; i++) {
		win_count[i] = 0;
	}
	
	float inputs[4096];
	float * output;
	
	nn p1(4096, 4096, 1);
	
	int result;
	
	std::cout<<"Load win/loss data\n";
	wl_data = fopen("results.dat", "r");
	while (std::fread(buffer, sizeof(int), 1, wl_data) >= 1) {
		win_count[buffer[0]]++;
	}
	select(win_count, selected);
	for (int i=0; i<4; i++) {
		std::cout<<selected[i]<<": "<<win_count[selected[i]]<<"\n";
	}
	
	std::cout<<"Generate new neural nets\n";
	for (int i=0; i<4; i++) {
		std::cout<<i+(4*std::stoi(argv[1]))<<"\n";
		p1.load_data("curr_gen/"+std::to_string(selected[std::stoi(argv[1])])+".dat");
		std::cout<<"  Mutating "<<i+(4*std::stoi(argv[1]))<<" from "<<selected[std::stoi(argv[1])]<<"\n";
		p1.mutate(4);
		std::cout<<"  Printing "<<i+(4*std::stoi(argv[1]))<<"\n";
		p1.print_data("new_gen/"+std::to_string(i+(4*std::stoi(argv[1])))+".dat");
	}
	
	std::fclose(wl_data);
	return 0;
}



void select(int win_count[16], int selected[4]) {
	int s;
	
	for (int i=0; i<4; i++) {
		selected[i] = i;
	}
	
	for (int i=4; i<16; i++) {
		s = smallest(win_count, selected);
		if (win_count[i] > s) {//If neural net i has a greater win/loss ratio than the smallest:
			for (int k=0; k<4; k++) {//Replace smallest
				//std::cout<<selected[i]<<"-"<<win_count[selected[i]]<<" vs "<<
				if (selected[k] == s) {
					selected[k] = i;
					break;
				}
			}
		}
	}
}

int smallest(int win_count[16], int selected[4]) {
	int smallest = selected[0];
	int s_wins = win_count[smallest];
	int curr_wins;
	
	for (int i=1; i<4; i++) {
		curr_wins = win_count[i];
		if (curr_wins < s_wins) {
			smallest = i;
			s_wins = curr_wins;
		}
	}
	
	return smallest;
}
