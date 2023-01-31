#include <iostream>
#include <string>
#include <fstream>
#include <random>

#include <bitset>
#include <cstring>

#include <thread>

class perceptron
{
	public:
		perceptron();
		perceptron(int numInputs);
		//Creates a perceptron with the given number of inputs and stores the pointer to the weights.
		perceptron(int numInputs, float * weights);
		
		//Creates a perceptron with the same m_numInputs as target
		//	Does NOT copy the weights or inputs
		perceptron& operator=(const perceptron & target);
		
		~perceptron();
		
		
		//Sets the pointer to the perceptron's inputs to the "data" parameter
		void set_inputs(float * data);
		
		//Sets the pointer to the perceptron's inputs to the "data" parameter
		//Sets the number of inputs to the "numInputs" patameter
		void set_inputs(int numInputs, float * data);
		
		
		//Sets the pointer to the weights of the inputs to the "weights" parameter
		void set_weights(float * weights);
		
		//Sets the contents of m_output to 1 if the threshold is met, 0 if not.
		void process();

		float * m_output;
		
		void set_numInputs(int numInputs);
		
		void mutate(const int & max_percent);
		void print_data(FILE * & fp);

	private:
		int m_numInputs;
		float * m_inputs;
		float * m_weights;
};

class nn {
	public:
		nn(const int & num_inputs, const int & num_outputs, const int & num_hidden_layers);
		~nn();
		
		
		void load_data(const std::string & filename);
		void print_data(const std::string & filename);
		
		void set_inputs(float * inputs);
		
		void process();
		
		void mutate(const int & max_percent);
		//void parallel_mutate(const int & max_percent, const int & num_cores);
		//void parallel_mutate_helper(const int & start, const int & end, const int & max_percent);
		
		//Pointer to array of inputs of size equal to m_num_inputs
		//NOT created automatically; must be manually set
		float * m_inputs;
		
		float * output();
		
	private:
		int m_num_inputs;
		int m_num_layers; //Input+hidden
		int m_num_outputs;
		perceptron ** m_mtrx;
		float ** m_internal_outputs;
		perceptron * m_output_layer;
		float * m_output;
};
#include "neural_net.hpp"
