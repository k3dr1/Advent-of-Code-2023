#include <fstream>
#include <iostream>
#include <vector>
#include <string>

using namespace std;

bool same(const vector<char>& v1, const vector<char>& v2) {
	if (v1.size() != v2.size()) {
		return false;
	} else {
		for (int idx = 0; idx < v1.size(); idx++) {
			if (v1[idx] != v2[idx]) {
				return false;
			}
		}
	}
	return true;
}


int similar(const vector<char>& v1, const vector<char>& v2, int edit_count) {
	if (v1.size() != v2.size()) {
		return -1;
	} else {
		for (int idx = 0; idx < v1.size(); idx++) {
			if (v1[idx] != v2[idx]) {
				edit_count--;
			}
			if (edit_count < 0) return edit_count;
		}
	}
	return edit_count;
}


char at(size_t i, size_t j, vector<vector<char>> grid){
	return grid.at(j).at(i);
}

vector<char> get_row(size_t j, vector<vector<char>> grid) {
	return grid.at(j);
}

vector<char> get_col(size_t i, vector<vector<char>> grid) {
	vector<char> res = {};
	for (int j = 0; j < grid.size(); j++) {
		res.push_back(grid.at(j).at(i));
	}
	return res;
}

bool ripple_h(int left, int right, vector<vector<char>> grid) {
	while (0 <= left && right < grid.size()) {
		if (!same(grid[left], grid[right])) {
			return false;
		}
		left--;
		right++;
	}
	return true;
}

bool ripple_v(int left, int right, vector<vector<char>> grid) {
	while (0 <= left && right < grid[0].size()) {
		if (!same(get_col(left, grid), get_col(right, grid))) {
			return false;
		}
		left--;
		right++;
	}
	return true;
}

// Returns the smaller index
int get_refl_horizontal(const vector<vector<char>>& grid) {
	vector<char> cur = grid[0];
	vector<char> next;
	for (int y = 0; y < grid.size() - 1; y++) {
		next = get_row(y+1, grid);
		if (same(cur, next)) {
			if (ripple_h(y, y+1, grid)) return y;
		}
		cur = next;
	}
	return -1;
}

// Returns the smaller index
int get_refl_vertical(const vector<vector<char>>& grid) {
	vector<char> cur = get_col(0, grid);
	vector<char> next;
	for (int x = 0; x < grid[0].size() - 1; x++) {
		next = get_col(x+1, grid);
		if (same(cur, next)) {
			if (ripple_v(x, x+1, grid)) return x;
		}
		cur = next;
	}
	return -1;
}

bool ripple_h2(int left, int right, vector<vector<char>> grid, int edit_count) {
	while (0 <= left && right < grid.size()) {
		edit_count = similar(grid[left], grid[right], edit_count);
		if (edit_count < 0) {
			return false;
		}
		left--;
		right++;
	}
	return (edit_count == 0);
}

bool ripple_v2(int left, int right, vector<vector<char>> grid, int& edit_count) {
	while (0 <= left && right < grid[0].size()) {
		edit_count = similar(get_col(left,grid), get_col(right,grid), edit_count);
		if (edit_count < 0) {
			return false;
		}
		left--;
		right++;
	}
	return (edit_count == 0);
}


// Returns the smaller index
int get_refl_horizontal2(const vector<vector<char>>& grid) {
	vector<char> cur = grid[0];
	vector<char> next;
	int edit_count;
	for (int y = 0; y < grid.size() - 1; y++) {
		edit_count = 1;
		next = get_row(y+1, grid);
		if (0 <= similar(cur, next, edit_count)) {
			if (ripple_h2(y, y+1, grid, edit_count)) return y;
		}
		cur = next;
	}
	return -1;
}

// Returns the smaller index
int get_refl_vertical2(const vector<vector<char>>& grid) {
	vector<char> cur = get_col(0, grid);
	vector<char> next;
	int edit_count;
	for (int x = 0; x < grid[0].size() - 1; x++) {
		edit_count = 1;
		next = get_col(x+1, grid);
		if (0 <= similar(cur, next, edit_count)) {
			if (ripple_v2(x, x+1, grid, edit_count)) {
				return x;
			}
		}
		cur = next;
	}
	return -1;
}




int main() {

	std::ifstream f("input.txt");

	if (!f.is_open()) {
		std::cout << "Coudln't open the file\n";
	}

	vector<vector<vector<char>>> grids;
	vector<vector<char>> grid;
	string line;
	while (getline(f, line)) {
		//std::cout << line << '\n';
		if (line.empty()) {
			grids.push_back(grid);
			grid.clear();
		} else {
			vector<char> row = {};
			for (char ch : line) row.push_back(ch);
			grid.push_back(row);
		}
	}
	if (!grid.empty()) grids.push_back(grid);

	long long int verts = 0;
	long long int horz = 0;
	for (int i = 0; i < grids.size(); i++) {
		std::cout << "i=" << i << '\n';
		grid = grids.at(i);
		long long v = get_refl_vertical2(grid) + 1;
		if (v != 0) {
			verts += v;
			std::cout << "v=" << v <<'\n';
		} else {
			long long h = get_refl_horizontal2(grid) + 1;
			horz += h;
			std::cout << "h=" << h << '\n';
		}
	}
	
	long long res = verts + 100*horz;

	std::cout << "The answer for part 1 is " << res << '\n';


	return 0;
}
