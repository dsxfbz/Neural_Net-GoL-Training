#ifndef GOL_HPP
#define GOL_HPP

GoL::GoL() {
	m_y_len = 64;
	m_x_len = 64;
	m_curr_board = new std::bitset<64>[m_y_len];
	m_next_board = new std::bitset<64>[m_y_len];
	
	m_L_win = false;
	m_R_win = false;
}

GoL::~GoL() {
	delete[] m_curr_board;
	delete[] m_next_board;
}

void GoL::reset() {
	for (int i=0; i<m_y_len; i++) {
		for (int k=0; k<m_x_len; k++) {
			m_curr_board[i][k] = 0;
			m_next_board[i][k] = 0;
		}
	}
	m_L_win = false;
	m_R_win = false;
}

void GoL::load_data(const std::bitset<64> * & source, const int & y_start, const int & y_end, const int & x_start, const int & x_end) {
	for (int i=y_start; i<y_end; i++) {
		for (int k=x_start; k<x_end; k++) {
			m_curr_board[i][k] = source[i][k];
		}
	}
	update_next();
}


std::bitset<64> * GoL::currBoard() {return m_curr_board;}
std::bitset<64> * GoL::nextBoard() {return m_next_board;}

void GoL::print(std::bitset<64> * target) {
	std::cout<<"+";
	for (int k=0; k<m_x_len; k++) {
		std::cout<<"-";
	}
	std::cout<<"+\n";
	for (int i=0; i<m_y_len; i++) {
		if (m_R_win) {
			std::cout<<"X";
		}
		else {
			std::cout<<"|";
		}
		
		for (int k=0; k<m_x_len; k++) {
			if (target[i][k]) {
				std::cout<<"█";
			}
			else {
				std::cout<<" ";
			}
		}
		
		if (m_L_win) {
			std::cout<<"X\n";
		}
		else {
			std::cout<<"|\n";
		}
	}
	std::cout<<"+";
	for (int k=0; k<m_x_len; k++) {
		std::cout<<"-";
	}
	std::cout<<"+\n";
}
void GoL::print() {
	print(m_curr_board);
}


int GoL::winner() {
	if (m_L_win) {
		if (m_R_win) {
			return 3;
		}
		return 1;
	}
	else if (m_R_win) {
		return 2;
	}
	return 0;
}

void GoL::nn_data(float data[4096]) {
	for (int i=0; i<m_y_len; i++) {
		for (int k=0; k<m_x_len; k++) {
			data[(i*m_y_len)+k] = m_next_board[i][k];
		}
	}
}
void GoL::nn_data_inv(float data[4096]) {
	for (int i=0; i<m_y_len; i++) {
		for (int k=0; k<m_x_len; k++) {
			data[((m_y_len-i-1)*m_x_len)+(m_x_len-k-1)] = m_next_board[i][k];
		}
	}
}

void GoL::extract_data(float data[4096], const int & x_start, const int & x_end) {
	for (int i=0; i<m_y_len; i++) {
		for (int k=x_start; k<x_end; k++) {
			m_curr_board[i][k] = data[(i*m_y_len)+k];
		}
	}
	update_next();
}
void GoL::extract_data_inv(float data[4096], const int & x_start, const int & x_end) {
	for (int i=0; i<m_y_len; i++) {
		for (int k=x_start; k<x_end; k++) {
			m_curr_board[i][k] = data[((m_y_len-i)*m_x_len)+(m_x_len-k)];
		}
	}
	update_next();
}

bool GoL::cell_step(const int & y, const int & x) {
	int sum = 0;
	
	if ((0 <= y-1) && (y-1 < m_y_len)) {
		if ((0 <= x-1) && (x-1 < m_x_len)) {
			sum += m_curr_board[y-1][x-1];
		}
		if ((0 <= x) && (x < m_x_len)) {
			sum += m_curr_board[y-1][x];
		}
		if ((0 <= x+1) && (x+1 < m_x_len)) {
			sum += m_curr_board[y-1][x+1];
		}
	}
	//if (0 <= y < m_y_len) {
		if ((0 <= x-1) && (x-1 < m_x_len)) {
			sum += m_curr_board[y][x-1];
		}
		if ((0 <= x+1) && (x+1 < m_x_len)) {
			sum += m_curr_board[y][x+1];
		}
	//}
	if ((0 <= y+1) && (y+1 < m_y_len)) {
		if ((0 <= x-1) && (x-1 < m_x_len)) {
			sum += m_curr_board[y+1][x-1];
		}
		if ((0 <= x) && (x < m_x_len)) {
			sum += m_curr_board[y+1][x];
		}
		if ((0 <= x+1) && (x+1 < m_x_len)) {
			sum += m_curr_board[y+1][x+1];
		}
	}
	
	if (m_curr_board[y][x] && (2 <= sum) && (sum <= 3)) {
		return true;
	}
	else if (sum==3) {
		return true;
	}
	return false;
}
void GoL::section_step(const int & y_start, const int & y_end) {
	for (int i=y_start; i<y_end; i++) {
		for (int k=0; k<m_x_len; k++) {
			m_next_board[i][k] = cell_step(i, k);
		}
	}
}
void GoL::update_next() {
	section_step(0, m_y_len);////
}
void GoL::board_step() {
	//Delete curr & Replace
	m_temp_board = m_curr_board;
	m_curr_board = m_next_board;
	
	//Update next board
	m_next_board = m_temp_board;
	update_next();
	
	//Update w/l
	if (!m_L_win && !m_R_win) {
		for (int i=0; i<m_y_len; i++) {
			if (m_curr_board[i][0]) {
				m_R_win = true;
			}
			if (m_curr_board[i][m_x_len-1]) {
				m_L_win = true;
			}
		}
	}
}



template <typename AI>
int run_game(AI & ai_1, AI & ai_2, GoL & game) {
	float * output;
	
	game.reset();
	
	std::cout<<"Revealed Section\n";
	game.nn_data(ai_1.m_inputs);
	game.nn_data_inv(ai_2.m_inputs);
	ai_1.process();
	ai_2.process();
	
	output = ai_1.output();
	game.extract_data(output, 16, 32);
	output = ai_2.output();
	game.extract_data_inv(output, 32, 48);
	
	std::cout<<"Hidden Section\n";
	game.nn_data(ai_1.m_inputs);
	game.nn_data_inv(ai_2.m_inputs);
	ai_1.process();
	ai_2.process();
	
	output = ai_1.output();
	game.extract_data(output, 8, 16);
	output = ai_2.output();
	game.extract_data(output, 48, 56);
	
	//Run game, turn Limit
	int temp = 0;
	for (int i=0; i<100; i++) {
		//std::cout<<"    Turn "<<i<<": "<<game.winner()<<"\n";
		if (game.winner() != 0) {
			break;
		}
		game.board_step();
		temp++;
	}
	std::cout<<"Turns: "<<temp<<"\n";
	return game.winner();
}

template <typename AI>
int run_game_manual(AI & ai_1, AI & ai_2, GoL & game) {
	float * output;
	
	game.reset();
	
	std::cout<<"Revealed Section\n";
	game.nn_data(ai_1.m_inputs);
	game.nn_data_inv(ai_2.m_inputs);
	ai_1.process();
	ai_2.process();
	
	output = ai_1.output();
	game.extract_data(output, 16, 32);
	output = ai_2.output();
	game.extract_data_inv(output, 32, 48);
	
	game.print();
	
	std::cout<<"Hidden Section\n";
	game.nn_data(ai_1.m_inputs);
	game.nn_data_inv(ai_2.m_inputs);
	ai_1.process();
	ai_2.process();
	
	output = ai_1.output();
	game.extract_data(output, 8, 16);
	output = ai_2.output();
	game.extract_data(output, 48, 56);
	
	game.print();
	
	//Run game, turn Limit
	int i = 0;
	std::string choice;
	while ((i<100) && (choice != "q")) {
	//for (int i=0; i<100; i++) {
		//std::cout<<"    Turn "<<i<<": "<<game.winner()<<"\n";
		choice = "";
		if (game.winner() != 0) {
			break;
		}
		game.board_step();
		
		game.print();
		std::cout<<"Menu:\n";
		std::cout<<"  Quit: 'q'\n";
		std::cout<<"  Next: Any other input\n";
		std::cout<<"Choice: ";
		std::cin>>choice;
		
		i++;
	}
	std::cout<<"Turns: "<<i<<"\n";
	return game.winner();
}
#endif
