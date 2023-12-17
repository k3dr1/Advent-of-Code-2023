#include <array>
#include <iostream>
#include <limits>
#include <queue>
#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <fstream>

using std::vector;
using std::string;
using std::ifstream;
using std::cout;

struct Node {
	int y;
	int x;
	std::array<int, 4> jmps;
	int g;
	int f;

	Node()=default;

	Node(int y_val, int x_val, std::array<int,4> jmps_val, int g_val, int f_val) 
	: y(y_val), x(x_val), jmps(jmps_val), g(g_val), f(f_val) {}

	bool operator<(const Node& rhs) const { 
		return f > rhs.f;
	}

	bool operator==(const Node& rhs) const {
		bool res = true;
		res &= (y == rhs.y);
		res &= (x == rhs.x);
		res &= (jmps[1] == rhs.jmps[1]);
		res &= (jmps[2] == rhs.jmps[2]);
		res &= (jmps[3] == rhs.jmps[3]);
		res &= (jmps[4] == rhs.jmps[4]);
		res &= (g == rhs.g);
		res &= (f == rhs.f);
		return res;
	}
};

template <>
struct std::hash<Node> {
	std::size_t operator()(const Node& n) const {
		std::string s = "" + to_string(n.y) + " " + to_string(n.x) + " " 
			+ to_string(n.jmps[0]) + " " + to_string(n.jmps[1]) + " " + to_string(n.jmps[2]) + " " + to_string(n.jmps[3]);
		return hash<string>()(s);
	}
};

int heuristic(const int y, const int x, const vector<string>& grid) {
	return std::abs(int(grid.size() - 1 - y)) + std::abs(int(grid.at(0).size() - 1 - x));
}

bool inside(Node& p, vector<string>& grid) {
	bool res = ((0 <= p.y && p.y < grid.size()) && (0 <= p.x && p.x < grid.at(0).size()));
	return res;
}

//bool bottom_right(const Node& p, vector<string>& grid) {
//	return (p.y == grid.size() - 1) && (p.x == grid.at(0).size() - 1);
//}
//
//void node_print(const Node& p) {
//	std::cout << "(" << p.x << "," << p.y << ") dir=" << p.dir << " rem_moves=" << p.remaining_moves << " mhl=" << p.heat_loss << '\n';
//}

void grid_print(const vector<string>& grid) {
	for (auto& line : grid) {
		std::cout << line << '\n';
	}
	return;
}

std::vector<Node> neighbors(Node u, vector<string>& grid) {
	// refresh the fucking jumps
	vector<Node> res;
	Node n0 = Node(u.y-1, u.x, u.jmps, u.g, 0);
	Node n1 = Node(u.y, u.x-1, u.jmps, u.g, 0);
	Node n2 = Node(u.y+1, u.x, u.jmps, u.g, 0);
	Node n3 = Node(u.y, u.x+1, u.jmps, u.g, 0);
	if (inside(n0, grid) && u.jmps[2] == 3) {
		n0.jmps[0] -= 1;
		if (0 <= n0.jmps[0]) {
			n0.jmps[1] = 3;
			n0.jmps[2] = 3;
			n0.jmps[3] = 3;
			n0.g += grid.at(n0.y).at(n0.x) - '0';
			n0.f = n0.g + heuristic(n0.y, n0.x, grid);
			res.push_back(n0);
		}
	}
	if (inside(n1, grid) && u.jmps[3] == 3) {
		n1.jmps[1] -= 1;
		if (0 <= n1.jmps[1]) {
			n1.jmps[0] = 3;
			n1.jmps[2] = 3;
			n1.jmps[3] = 3;
			n1.g += grid.at(n1.y).at(n1.x) - '0';
			n1.f = n1.g + heuristic(n1.y, n1.x, grid);
			res.push_back(n1);
		}
	}
	if (inside(n2, grid) && u.jmps[0] == 3) {
		n2.jmps[2] -= 1;
		if (0 <= n2.jmps[2]) {
			n2.jmps[0] = 3;
			n2.jmps[1] = 3;
			n2.jmps[3] = 3;
			n2.g += grid.at(n2.y).at(n2.x) - '0';
			n2.f = n2.g + heuristic(n2.y, n2.x, grid);
			res.push_back(n2);
		}
	}
	if (inside(n3, grid) && u.jmps[1] == 3) {
		n3.jmps[3] -= 1;
		if (0 <= n3.jmps[3]) {
			n3.jmps[0] = 3;
			n3.jmps[1] = 3;
			n3.jmps[2] = 3;
			n3.g += grid.at(n3.y).at(n3.x) - '0';
			n3.f = n3.g + heuristic(n3.y, n3.x, grid);
			res.push_back(n3);
		}
	}
	return res;
}

#define  WIDTH 13
#define HEIGHT 13

int pathfind(vector<string> grid) {
	std::priority_queue<Node> queue;
	std::unordered_map<Node, Node> visited;
	bool stop = false;
	//std::array<std::array<bool, WIDTH>, HEIGHT> unvisited;

	// Start node
	queue.push(Node(0, 0, std::array<int,4>{3,3,3,3}, 0, 0));

	while (!queue.empty() && !stop) {
		Node u = queue.top();
		queue.pop();

		//std::cout << "x=" << u.x << " y=" << u.y << " f=" << u.f << '\n';
		if (u.y == grid.size() - 1 && u.x == grid.at(0).size() - 1) {
			stop = true;
			return u.g;
		}

		for (Node v : neighbors(u, grid)) {
			if (visited.find(v) == visited.end()) {
				//if (v.y == 12 && v.x == 12) stop = true;
				visited[v] = v;
				queue.push(v);
			} else {
				// Already in the visited
				// v and alt_v have the same jmps, only different weights
				Node alt_v = visited.at(v);
				if (v.f < alt_v.f) {
					visited.at(v) = v;
				}
			}
		}
	}

	return -1;
}

//int min_heat_loss(vector<string> grid) {
//	vector<string> v_grid = grid;
//
//	vector<Node> p_queue;
//	std::unordered_map<string, int> visited;
//
//	int min_heat = 0;
//	bool found = false;
//	p_queue.push_back(Node(0,0,-1,-1,3,3,0));
//	visited[path_to_str(Node(0,0,-1,-1,3,3,0))] = 0;
//	while (!found) {
//		// s = smallest
//		Node s = pop_smallest_cost_path(p_queue, grid);
//		node_print(s);
//		//v_grid.at(s.y).at(s.x) = '#';
//		path_print(s, grid);
//
//		//Debug
//		//std::cout << "Checking Path \n";
//
//		for (int d = 0; d < 4; d++) {
//		if (d == s.dir)  {
//				if (0 < s.remaining_moves) {
//					Node next = Node(s.y, s.x, s.y, s.x, s.dir, s.remaining_moves- 1, s.heat_loss);
//					move(next);
//					if (inside(next, grid)){ 
//						next.heat_loss += (grid.at(next.y).at(next.x) - '0');
//						if (visited.find(path_to_str(next)) == visited.end() 
//							|| (next.heat_loss < visited[path_to_str(next)] && next.dir == visited[path_to_str(next)])) {
//							if (bottom_right(next, grid)) {
//								found = true;
//								min_heat = next.heat_loss;
//							}
//							p_queue.push_back(next);
//							visited[path_to_str(next)] = next.heat_loss;
//						}
//					}
//				}
//			} else {
//				Node next = Node(s.y, s.x, s.y, s.x, d, 3, s.heat_loss);
//				move(next);
//				if ( inside(next, grid)) { 
//					next.heat_loss += (grid.at(next.y).at(next.x) - '0');
//					if (visited.find(path_to_str(next)) == visited.end() 
//						|| (next.heat_loss < visited[path_to_str(next)] && next.dir == visited[path_to_str(next)]))  {
//						if (bottom_right(next, grid)) {
//							found = true;
//							min_heat = next.heat_loss;
//						}
//						p_queue.push_back(next);
//						visited[path_to_str(next)] = next.heat_loss;
//					}
//				}
//			}
//		}
//	}
//	return min_heat;
//}



int main() {

	ifstream f("input.txt");

	if (!f.is_open()) {
		std::cout << "Couldnt open the file\n";
		return 1;
	}

	vector<string> grid;
	string line;
	while (getline(f, line)) {
		std::cout << line << '\n';
		grid.push_back(line);
	}

	//int res = min_heat_loss(grid);
	int res = pathfind(grid);

	std::cout << "The answer for part 1 is " << res << '\n';

	return 0;
}
