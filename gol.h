#ifndef GOL_H
#define GOL_H

#include <bitset>
#include <cstring>

//bool GoL_step(const bitset * & board, const int & y_size, const int & x_size, const int & y, const int & x) {}


//Default board size: 32x64, with 4 32x16 sections
class GoL {
	public:
		GoL();
		~GoL();
		
		//Clears current board, resets win trackers
		void reset();
		
		//Loads a region of the board "source," from start and up to but not including end.
		void load_data(const std::bitset<64> * & source, const int & y_start, const int & y_end, const int & x_start, const int & x_end);
		
		//Feed into NN in inverted order for opposite sides
		std::bitset<64> * currBoard();
		std::bitset<64> * nextBoard();
		
		void print(std::bitset<64> * target);
		void print();
		
		/*
		0: None
		1: Left
		2: Right
		3: Draw
		*/
		int winner();
		
		void nn_data(float data[4096]);
		void nn_data_inv(float data[4096]);
		void extract_data(float data[4096], const int & x_start, const int & x_end);
		void extract_data_inv(float data[4096], const int & x_start, const int & x_end);
		
		//Swaps m_curr_board with m_next_board, then updates m_next_board
		//Updates winner status
		void board_step();
		
	private:
		int m_y_len;
		int m_x_len;
		std::bitset<64> * m_curr_board;
		std::bitset<64> * m_next_board;
		std::bitset<64> * m_temp_board;
		bool m_L_win;
		bool m_R_win;
		
		void make_board(std::bitset<64> * first, std::bitset<64> * next);
		void del_board(std::bitset<64> * target);
		
		//Tells whether cell should be alive next step based on m_curr_board
		bool cell_step(const int & y, const int & x);
		//Updates rows [y_start, y_end) in m_next_board
		void section_step(const int & y_start, const int & y_end);
		void update_next();
		
		void full_run();
};



//Returns a number representing the result of the game
/*
0: Ran out of turns
1: ai_1 wins
2: ai_2 wins
3: Draw
*/
template <typename AI>
int run_game(AI & ai_1, AI & ai_2, GoL & game);
template <typename AI>
int run_game_manual(AI & ai_1, AI & ai_2, GoL & game);

#include "gol.hpp"
#endif
