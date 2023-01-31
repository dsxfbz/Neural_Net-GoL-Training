perceptron::perceptron(int numInputs) {
	m_numInputs = numInputs;
	m_inputs = nullptr;
	m_weights = nullptr;
	m_output = nullptr;
}

perceptron::perceptron(int numInputs, float * weights) {
	m_numInputs = numInputs;
	m_inputs = nullptr;
	m_weights = weights;
	m_output = nullptr;
}


perceptron::perceptron() {
	m_numInputs = 0;
	m_inputs = nullptr;
	m_weights = nullptr;
	m_output = nullptr;
}

perceptron& perceptron::operator=(const perceptron & target) {
	m_numInputs = target.m_numInputs;
	m_inputs = nullptr;
	m_weights = nullptr;

	return *this;
}

perceptron::~perceptron() {delete[] m_weights;}

void perceptron::set_inputs(int numInputs, float * data) {
	m_numInputs = numInputs;
	m_inputs = data;
}

void perceptron::set_inputs(float * data) {m_inputs = data;}

void perceptron::set_weights(float * weights) {
	delete[] m_weights;
	m_weights = weights;
}

void perceptron::set_numInputs(int numInputs) {m_numInputs = numInputs;}

void perceptron::process() {
	float sum = 0;
	for(int i=0; i<m_numInputs; i++)
	{
		sum += (m_inputs[i] * m_weights[i]);
	}
	
	if(-1 < sum)
	{
		*m_output = 1;
	}
	else
	{
		*m_output = 0;
	}
	return;
}

void perceptron::mutate(const int & max_percent) {
	float change;
	for (int i=0; i<m_numInputs; i++) {
		change = static_cast<float>(rand()%max_percent)/100;
		if (rand()%2) {
			m_weights[i] += change;
		}
		else {
			m_weights[i] -= change;
		}
	}
}

void perceptron::print_data(FILE * & fp) {
	fwrite(m_weights, sizeof(float), m_numInputs, fp);
}


nn::nn(const int & num_inputs, const int & num_outputs, const int & num_hidden_layers) {
	m_num_inputs = num_inputs;
	m_inputs = nullptr;
	m_num_outputs = num_outputs;
	m_num_layers = num_hidden_layers+1;
	
	//Make Matrix
	m_mtrx = new perceptron * [m_num_layers];
	m_internal_outputs = new float * [m_num_layers];
	for (int i=0; i<m_num_layers; i++) {
		//Build row
		m_mtrx[i] = new perceptron [m_num_inputs];
		
		//Build pointers to outputs
		m_internal_outputs[i] = new float [m_num_inputs];
		for (int k=0; k<m_num_inputs; k++) {
			m_mtrx[i][k].m_output = &(m_internal_outputs[i][k]);
		}
		
		//Connect to inputs or prev. layer
		if (i>0) {
			for (int k=0; k<m_num_inputs; k++) {
				m_mtrx[i][k].set_inputs(m_num_inputs, m_internal_outputs[i-1]);
			}
		}
		else {
			for (int k=0; k<m_num_inputs; k++) {
				m_mtrx[0][k].set_inputs(m_num_inputs, m_inputs);
			}
		}
	}
	
	//Make outputs 
	//Build row
	m_output_layer = new perceptron [m_num_outputs];
	//Build pointers to outputs
	m_output = new float [m_num_outputs];
	for (int k=0; k<m_num_outputs; k++) {
		m_output_layer[k].m_output = &(m_output[k]);
	}
	//Connect to prev. layer
	for (int k=0; k<m_num_outputs; k++) {
		m_output_layer[k].set_inputs(m_num_inputs, m_internal_outputs[m_num_layers-1]);
	}
}

nn::~nn() {
	//delete[] m_inputs;
	
	for (int i=0; i<m_num_layers; i++) {
		delete[] m_mtrx[i];
		delete[] m_internal_outputs[i];
	}
	delete[] m_mtrx;
	delete[] m_internal_outputs;
	
	delete[] m_output_layer;
	delete[] m_output;
}

void nn::load_data(const std::string & filename) {
	FILE * input;
	input = std::fopen(filename.c_str(), "r");
	
	float * curr_w_set;
	
	//Input & hidden matrix
	//Layer
	for (int i=0; i<m_num_layers; i++) {
		//Perceptron
		for (int k=0; k<m_num_inputs; k++) {
			//std::cout<<"      layer "<<i<<" perceptron "<<k<<"\n";
			curr_w_set = new float [m_num_inputs];
			std::fread(curr_w_set, sizeof(float), m_num_inputs, input);
			m_mtrx[i][k].set_weights(curr_w_set);
		}
	}
	//Output layer
	//Perceptron
	for (int k=0; k<m_num_outputs; k++) {
		curr_w_set = new float [m_num_inputs];
		std::fread(curr_w_set, sizeof(float), m_num_inputs, input);
		m_output_layer[k].set_weights(curr_w_set);
	}
	
	std::fclose(input);
}

void nn::print_data(const std::string & filename) {
	FILE * fp;
	fp = fopen(filename.c_str(), "w");
	
	//Input & hidden matrix
	//Layer
	for (int i=0; i<m_num_layers; i++) {
		//Perceptron
		for (int k=0; k<m_num_inputs; k++) {
			m_mtrx[i][k].print_data(fp);
		}
	}
	//Output layer
	//Perceptron
	for (int k=0; k<m_num_inputs; k++) {
		m_output_layer[k].print_data(fp);
	}
	std::fclose(fp);
}

void nn::set_inputs(float * inputs) {
	m_inputs = inputs;
	for (int i=0; i<m_num_inputs; i++) {
		m_mtrx[0][i].set_inputs(inputs);
	}
}

void nn::process() {
	for (int i=0; i<m_num_layers; i++) {
		for (int k=0; k<m_num_inputs; k++) {
			//std::cout<<"nn::process() ["<<i<<"]["<<k<<"]\n";
			m_mtrx[i][k].process();
		}
	}
	for (int k=0; k<m_num_outputs; k++) {
		m_output_layer[k].process();
	}
}
void nn::mutate(const int & max_percent) {
	for (int i=0; i<m_num_layers; i++) {
		for (int k=0; k<m_num_inputs; k++) {
			m_mtrx[i][k].mutate(max_percent);
		}
	}
	for (int k=0; k<m_num_outputs; k++) {
		m_output_layer[k].mutate(max_percent);
	}
}
/*
void nn::parallel_mutate(const int & max_percent, const int & num_cores) {
	std:: thread t[num_cores];
	int start;
	int end;
	
	for (int k=0; k<num_cores; k++) {
		start = k*num_cores;
		end = (k+1)*num_cores+1;
		t[k] = std::thread(&nn::parallel_mutate_helper, std::ref(start), std::ref(end), std::ref(max_percent));
	}
	for (int i=0; i<num_cores; i++) {
		t[i].join();
	}
	for (int k=0; k<m_num_outputs; k++) {
		m_output_layer[k].mutate(max_percent);
	}
}
void nn::parallel_mutate_helper(const int & start, const int & end, const int & max_percent) {
	for (int i=0; i<m_num_layers; i++) {
		for (int k=start; k<(end); k++) {
			m_mtrx[i][k].mutate(max_percent);
		}
	}
	for (int k=0; k<m_num_outputs; k++) {
		m_output_layer[k].mutate(max_percent);
	}
}
*/

float * nn::output() {return m_output;}
