#include <iostream> 
#include <sstream>
#include <unordered_map>
#include <vector>
#include <string>
#include <fstream>

using std::vector;
using std::string;

void print_grid(vector<string>& grid) {
	for (auto line : grid) {
		std::cout << line << '\n';
	}
}

bool compare_grids(vector<string>& grid1, vector<string>& grid2) {
	for (int y = 0; y < grid1.size(); y++) {
		if (grid1.at(y).compare(grid2.at(y)) != 0) return false;
	}
	return true;
}

// 0 North
// 1 West
// 2 South
// 3 East
void roll(vector<string>& grid, int direction) {
	vector<string> before_grid = grid;
	int placex;
	int placey;

	// North & West
	if (direction == 0 || direction == 1) {
		for (int x = 0; x < grid[0].size(); x++) {
			for (int y = 0; y < grid.size(); y++) {
				if (grid.at(y).at(x) == 'O') {
					placex = x;
					placey = y;
					switch (direction) {
						// North
						case 0:
							while (placey > 0 && grid.at(placey-1).at(x) == '.') {placey--;}
							grid.at(y).at(x) = '.';
							grid.at(placey).at(placex) = 'O';
						break;
						// West
						case 1: 
							while (placex > 0 && grid.at(y).at(placex-1) == '.') {placex--;}
							grid.at(y).at(x) = '.';
							grid.at(placey).at(placex) = 'O';
						break;
					}
				}
			}
		}
	}


	// South & East
	if (direction == 2 || direction == 3) {
		for (int x = grid[0].size() - 1; 0 <= x; x--) {
			for (int y = grid[0].size() - 1; 0 <= y; y--) {
				if (grid.at(y).at(x) == 'O') {
					placex = x;
					placey = y;
					switch (direction) {
						// South
						case 2:
							while (placey < grid.size() - 1 && grid.at(placey+1).at(x) == '.') {placey++;}
							grid.at(y).at(x) = '.';
							grid.at(placey).at(placex) = 'O';
						break;
						// East
						case 3:
							while (placex < grid[0].size() - 1 && grid.at(y).at(placex+1) == '.') {placex++;}
							grid.at(y).at(x) = '.';
							grid.at(placey).at(placex) = 'O';
						break;
					}
				}
			}
		}
	}
	return;
}

int main() {
	
	std::ifstream f("input.txt");

	if (!f.is_open()) {
		std::cout << "Couldnt open the file\n";
	}

	vector<string> grid;
	string line;

	while (std::getline(f,line)) {
		grid.push_back(line);
	}


	int grid_height = grid.size();

	vector<string> initial_grid = grid;
	vector<string> check_grid = grid;
	int match = 0;
	int loop = 0;

	// Part 2
	std::unordered_map<string, string> memo;
	for (int roll_count = 1; roll_count <= 1'000'000'000; roll_count++) {
		roll(grid, 0);
		roll(grid, 1);
		roll(grid, 2);
		roll(grid, 3);
		if (roll_count%1000 == 0) {
			if (roll_count == 10000) {
				check_grid = grid;
			}
			std::cout << "After roll = " << roll_count << '\n';
			continue;
		}

		if (compare_grids(check_grid,grid)) {
			std::cout << "Arrived at the same place!\n";
			std::cout << "Same at roll count = " << roll_count << '\n';
			match = roll_count;
			loop = roll_count - 10000;
			std::cout << "Loop length = " << loop << '\n';
			break;
		}
	}

	int remaining_rolls = (1'000'000'000 - match)%loop;
	std::cout << "Remaining rolls = " << remaining_rolls << '\n';

	for (int roll_count = 1; roll_count <= remaining_rolls; roll_count++) {
		roll(grid, 0);
		roll(grid, 1);
		roll(grid, 2);
		roll(grid, 3);
	}

	int load2 = 0;

	for (int x = 0; x < grid[0].size(); x++){
		for (int y = 0; y < grid.size(); y++) {
			if (grid.at(y).at(x) == 'O') {
				load2 += grid_height - y;
			}
		}
	}

	int load1 = 0;

	// Part 1
	for (int x = 0; x < initial_grid[0].size(); x++){
		int highest_possible = 0;
		for (int y = 0; y < initial_grid.size(); y++) {
			if (initial_grid.at(y).at(x) == 'O') {
				load1 += grid_height - highest_possible;
				highest_possible++;
			} else if (initial_grid.at(y).at(x) == '#') {
				highest_possible = y+1;
			}
		}
	}

	std::cout << "The answer for part 1 is " << load1 << '\n';
	std::cout << "The answer for part 2 is " << load2 << '\n';

	return 0;
}
