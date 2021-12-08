#include <iostream>
#include <vector>
#include <string>
#include algorithm>

const char EMPTY_CELL = '#';
const char PLAYER1_CELL = 'X';
const char PLAYER2_CELL = 'O';
int turns;

std::vector<std::string> stringSplit(std::string str, const std::string& delimiter) {
	// Utility to split string using delimiter
	std::vector<std::string> out{};
	size_t pos = 0;

	while ((pos = str.find(delimiter)) != std::string::npos) {
		out.push_back(str.substr(0, pos));
		str.erase(0, pos + delimiter.length());
	}

	return out;
}

bool isNumber(const std::string& s) {
	// Utility to check if given string is an integer
	std::string::const_iterator it = s.begin();
	while (it != s.end() && std::isdigit(*it)) ++it;
	return !s.empty() && it == s.end();
}

void printGrid(std::vector<std::vector<char>>& grid) {
	// Pretty print entire grid
	int i = 0;

    std::cout << "  1   2   3" << std::endl;

	for (std::vector<char> item: grid) {
		std::cout << std::to_string(i + 1) + " " + std::string(1, item[0]) + " | " + std::string(1, item[1]) + " | " + std::string(1, item[2]) << std::endl;
		if (i != 2) std::cout << " -----------" << std::endl;
		i++;
	}
}

char checkWinner(std::vector<std::vector<char>>& grid) {
	// Check Rows
	for (const std::vector<char>& item : grid) {
		if (item[0] == item[1] && item[1] == item[2]) {
			return item[0];
		}
	}

	// Check Columns
	for (int i = 0; i < 3; i++) {
		if (grid[0][i] == grid[1][i] && grid[1][i] == grid[2][i]) {
			return grid[0][i];
		}
	}

	// Check Diagonal
	if (grid[0][0] == grid[1][1] && grid[1][1] == grid[2][2]) {
		return grid[0][0];
	}

	// Reverse Diagonal
	if (grid[0][2] == grid[1][1] && grid[1][1] == grid[2][0]) {
		return grid[0][2];
	}
	
	// No winner yet
	return EMPTY_CELL;
}

void makeMove(std::vector<std::vector<char>>& grid, const char& player) {
	bool success = false;

	while (!success) {
		std::string in;

		std::cout << "Player " + std::string(1, player) + " turn: ";
		std::getline(std::cin, in);
		in += " -";
		std::vector<std::string> words = stringSplit(in, " ");

		if (words.size() >= 2) {
			std::string strrow = words[0];
			std::string strcolumn = words[1];

			if (isNumber(strcolumn) && isNumber(strrow)) {
				int column = std::stoi(strcolumn);
				int row = std::stoi(strrow);

				if (column >= 1 && column <= 3 && row >= 1 && row <= 3) {
					if (grid[column - 1][row - 1] == EMPTY_CELL) {
						success = true;
						grid[column - 1][row - 1] = player;
					}
					else {
						std::cout << "That cell is already occupied!" << std::endl;
					}
				}
				else {
					std::cout << "Both inputs must be between 1 and 3 inclusive!" << std::endl;
				}
			}
			else {
				std::cout << "Both inputs must be numbers!" << std::endl;
			}
		} else {
			std::cout << "Please enter 2 numbers!" << std::endl;
		}
	}
}

class Computer {
    public:
        int minimax(std::vector<std::vector<char>>& grid, int depth, bool isMax) {
            char winner = checkWinner(grid);
            
            if (winner == PLAYER1_CELL) return 10;
            if (winner == PLAYER2_CELL) return -10;
            if (turns >= 9) return 0;
            
            if (isMax) {
                int best = -1000;
                
                for (int i = 0; i < 3; i++) {
                    for (int j = 0; j < 3; j++) {
                        if (grid[i][j] == EMPTY_CELL) {
                            grid[i][j] = PLAYER2_CELL;
                            best = std::max(best, this->minimax(grid, depth + 1, !isMax));
                            grid[i][j] = EMPTY_CELL;
                        }
                    }
                }
                
                return best;
            } else {
                int best = 1000;
                
                for (int i = 0; i < 3; i++) {
                    for (int j = 0; j < 3; j++) {
                        if (grid[i][j] == EMPTY_CELL) {
                            grid[i][j] = PLAYER1_CELL;
                            best = std::max(best, this->minimax(grid, depth + 1, !isMax));
                            grid[i][j] = EMPTY_CELL;
                        }
                    }
                }
                
                return best;
            }
        }
        
        std::pair<int, int> bestMove(std::vector<std::vector<char>>& grid) {
            int bestValue = -1000;
            std::pair<int, int> best_move{-1, -1};
            
            for (int i = 0; i < 3; i++) {
                for (int j = 0; j < 3; j++) {
                    if (grid[i][j] == EMPTY_CELL) {
                        grid[i][j] = PLAYER2_CELL;
                        int moveValue = this->minimax(grid, 0, false);
                        grid[i][j] = EMPTY_CELL;
                        
                        if (moveValue > bestValue) {
                            best_move = {i, j};
                            bestValue = moveValue;
                        }
                    }
                }
            }
            
            return best_move;
        }
};

int main() {
	char playerTurn = PLAYER1_CELL;
	Computer comp;
	bool againstComputer = false;
	turns = 0;
	std::vector<std::vector<char>> grid = {
		{EMPTY_CELL, EMPTY_CELL, EMPTY_CELL},
		{EMPTY_CELL, EMPTY_CELL, EMPTY_CELL},
		{EMPTY_CELL, EMPTY_CELL, EMPTY_CELL}
	};
	
	char choice;
	std::cout << "Would you like to play against a computer? (y/n): ";
	std::cin >> choice;
	againstComputer = choice == 'y';
	
	printGrid(grid);

	while (1) {
		makeMove(grid, playerTurn);
		printGrid(grid);

		turns++;
		char winner = checkWinner(grid);

		if (winner != EMPTY_CELL) {
			std::cout << "Player " + std::string(1, playerTurn) + " wins!" << std::endl;
			break;
		} else {
			if (turns == 9) {
				std::cout << "It's a tie!" << std::endl;
				break;
			}
 		}

		playerTurn = playerTurn == PLAYER1_CELL ? PLAYER2_CELL : PLAYER1_CELL;
	}

	return 0;
}
