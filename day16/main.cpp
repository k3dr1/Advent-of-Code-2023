#include <iostream>
#include <unordered_map>
#include <vector>
#include <fstream>
#include <string>

using namespace std;

// 0 - UP
// 1 - LEFT
// 2 - DOWN
// 3 - RIGHT
struct Photon {
	int y;
	int x;
	int dir;

	// Constructor
	Photon(int y_val, int x_val, int dir_val)
	: y(y_val), x(x_val), dir(dir_val) {}

};

string dir_to_str(int dir) {
	switch (dir) {
		case 0:
			return "up";
		case 1:
			return "left";
		case 2:
			return "down";
		case 3:
			return "right";
	}
	return "broken";
}

void photon_print(Photon& p) {
	std::cout << "    y = " << p.y << ", x = " << p.x << ", dir = " << dir_to_str(p.dir) << '\n';
}

void stack_print(vector<Photon> stack) {
	std::cout << "Current stack: \n";
	for (auto& p : stack) {
		photon_print(p);
	}
}

void grid_print(vector<string>& grid, Photon& p) {
	std::cout << '\n';
	for (int y = 0; y < grid.size(); y++) {
		for (int x = 0; x < grid.at(0).size(); x++) {
			if (!(x == p.x && y == p.y)) {
				std::cout << grid.at(y).at(x);
			} else {
				std::cout << '$';
			}
		}
		std::cout << '\n';
	}
	std::cout << '\n';
	return;
}

bool inside(Photon& p, vector<string>& grid) {
	return ((0 <= p.y && p.y < grid.size()) && (0 <= p.x && p.x < grid.at(0).size()));
}

void move(Photon& p) {
	switch (p.dir) {
		case 0:
			p.y -= 1;
		break;
		case 1:
			p.x -= 1;
		break;
		case 2:
			p.y += 1;
		break;
		case 3:
			p.x += 1;
		break;
	}
	return;
}

int count(vector<string>& energized_grid) {
	int res = 0;
	for (auto& line : energized_grid) {
		for (auto& ch : line) {
			res += (ch == '#');
		}
	}
	return res;
}

struct Reflector_memo {
	bool up;
	bool left;
	bool down;
	bool right;

	Reflector_memo() :
	up(false), left(false), down(false), right(false) {}
};

vector<string> get_energized(Photon starting_photon, vector<string>& grid) {
	// Stores incoming angles of previous rays
	std::unordered_map<string, Reflector_memo> memo;
	vector<string> energized_grid;
	string empty_line = "";

	// Constructing the energy grid
	for (auto& ch : grid.at(0)) empty_line += ".";
	for (int i = 0; i < grid.size(); i++) energized_grid.push_back(empty_line);

	vector<Photon> stack;
	stack.push_back(starting_photon);

	while (!stack.empty()) {
		Photon p = stack.at(stack.size() - 1);
		stack.pop_back();
		while (inside(p, grid) && p.dir != -1) {

			//DEBUG
			//grid_print(energized_grid, p);
			//std::cout << "Current photon is: \n";
			//photon_print(p);
			//stack_print(stack);
			//std::cout << '\n';

			char tile = grid.at(p.y).at(p.x);
			energized_grid.at(p.y).at(p.x) = '#';
			switch (tile) {
				case '.':
					move(p);
					break;
				case '/':
					switch (p.dir) {
						case 0:
							p.dir = 3;
							move(p);
							break;
						case 1:
							p.dir = 2;
							move(p);
							break;
						case 2:
							p.dir = 1;
							move(p);
							break;
						case 3:
							p.dir = 0;
							move(p);
							break;
					}
					break;
				case '\\':
					switch (p.dir) {
						case 0:
							p.dir = 1;
							move(p);
							break;
						case 1:
							p.dir = 0;
							move(p);
							break;
						case 2:
							p.dir = 3;
							move(p);
							break;
						case 3:
							p.dir = 2;
							move(p);
							break;
					}
					break;
				case '|':
					if (p.dir == 1 || p.dir == 3) {
						if (!memo[to_string(p.x) + " " + to_string(p.y)].left 
								&& !memo[to_string(p.x) + " " + to_string(p.y)].right) {
							stack.push_back(Photon(p.y,p.x,2));
							p.dir = 0;
							memo[to_string(p.x) + " " + to_string(p.y)].left = true;
							memo[to_string(p.x) + " " + to_string(p.y)].right = true;
						} else {p.dir = -1;}
						move(p);
					} else {
						move(p);
					}
					break;
				case '-':
					if (p.dir == 0 || p.dir == 2) {
						if (!memo[to_string(p.x) + " " + to_string(p.y)].up 
								&& !memo[to_string(p.x) + " " + to_string(p.y)].down) {
							stack.push_back(Photon(p.y,p.x,3));
							p.dir = 1;
							memo[to_string(p.x) + " " + to_string(p.y)].up = true;
							memo[to_string(p.x) + " " + to_string(p.y)].down = true;
						} else {p.dir = -1;}
						move(p);
					} else {
						move(p);
					}
					break;
			}
		}
	}
	return energized_grid;
}

int main() {

	ifstream f("input.txt");

	if (!f.is_open()) {
		std::cout << "Coudlnt open the file\n";
		return 1;
	}

	vector<string> grid;
	string line;

	while (getline(f, line)) {
		grid.push_back(line);
	}

	// Part 1
	vector<string> energy_grid = get_energized(Photon(0,0,3), grid);
	std::cout << "Part 1 answer is " << count(energy_grid) << '\n';

	// Part 2
	int res = 0;
	for (int x = 0; x < grid.at(0).size(); x++) {
		energy_grid = get_energized(Photon(0, x, 2), grid);
		res = std::max(res, count(energy_grid));
		energy_grid = get_energized(Photon(grid.size() - 1, x, 0), grid);
		res = std::max(res, count(energy_grid));
	}
	for (int y = 1; y < grid.size() - 1; y++) {
		energy_grid = get_energized(Photon(y, 0, 3), grid);
		res = std::max(res, count(energy_grid));
		energy_grid = get_energized(Photon(y, grid.size() - 1, 1), grid);
		res = std::max(res, count(energy_grid));
	}

	std::cout << "Part 2 answer is " << res << '\n';

	for (auto& line : energy_grid) {
		std::cout << line << '\n';
	}

	return 0;
}
