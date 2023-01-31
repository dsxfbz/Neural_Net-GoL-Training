/*
 * run_game.cpp
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

//Runs one generation of games
//Args: [number of processers]

#include <iostream>
#include <cstring>

#include "gol.h"
#include "neural_net.h"


int main(int argc, char **argv)
{
	GoL game;
	
	float * inputs = new float[4096];
	float * output;
	
	nn p1(4096, 4096, 1);
	nn p2(4096, 4096, 1);
	
	FILE * results;
	results = std::fopen("results.dat", "a+");
	
	int * winner = new int[1];
	
	//Load neural net data
	
	std::cout<<"    Loading data "<<"curr_gen/"<<std::stoi(argv[1])<<".dat"<<"\n";
	p1.load_data(std::string("curr_gen/")+argv[1]+".dat");
	std::cout<<"    Loading data "<<"curr_gen/"<<std::stoi(argv[2])<<".dat"<<"\n";
	p2.load_data(std::string("curr_gen/")+argv[2]+".dat");
	
	game.reset();
	
	p1.set_inputs(inputs);
	p2.set_inputs(inputs);
	p1.process();
	p2.process();
	
	winner[0] = -1;
	switch (run_game_manual<nn>(p1, p2, game)) {
		case 0://Incomplete
			std::cout<<"UNFINISHED"<<"\n";
			break;
		case 1://L side wins
			*winner = std::stoi(argv[1]);
			std::cout<<"WINNER: "<<winner[0]<<"\n";
			break;
		case 2://R side wins
			*winner = std::stoi(argv[2]);
			std::cout<<"WINNER: "<<winner[0]<<"\n";
			break;
		case 3://Draw
			std::cout<<"DRAW\n";
			break;
		default:
			std::cout<<"ERROR\n";
	}
	
	game.print();
	
	std::fclose(results);
	delete[] inputs;
	delete[] winner;
	return 0;
}

