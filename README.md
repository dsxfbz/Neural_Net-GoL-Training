# Neural_Net-GoL-Training

To train:
	First, run gen_seed.exe once. This creates a set of starting files for the program to load into the neural nets. Use the command "./gen_seed.exe".
	Then, run train.sh to train the networks contained within. The variable "num_gen" defined within determines how many generations to run.

To run one round of the game, turn by turn:
	Use the command "./run_game.exe i k" with i and k as numbers from 0 to 15 to select which network to use.
