#include <algorithm>
#include <fstream>
#include <iostream>
#include <ostream>
#include <sstream>
#include <unordered_map>
#include <utility>
#include <vector>
#include <string>

// Assume that there are no ?s, only . and #
std::vector<long long int> get_nums(std::string s) {

	std::vector<long long int> res = {};
	long long int n = 0;
	for (long long int i = 0; i < s.size(); i++){
		if (s[i] == '#') n++;
		if (s[i] == '.') {
			if (n > 0) res.push_back(n);
			n = 0;
		}
	}

	if (n>0) res.push_back(n);

	return res;
}

long long int compare_nums(std::vector<long long int> n1, std::vector<long long int> n2){
	if (n1.size() != n2.size()) return 0;
	for (long long int i = 0; i < n1.size(); i++){
		if (n1[i] != n2[i]) return 0;
	}
	return 1;
}

long long int arrangements(std::string s, std::vector<long long int> should_nums){
	if (s.find('?') == std::string::npos) {
		if (compare_nums(get_nums(s), should_nums) == 1) {
			return 1;
		} else {
			return 0;
		}
	} else {
		long long int q_pos = s.find('?');
		std::string empt = s;
		std::string brok = s;
		empt[q_pos] = '.';
		brok[q_pos] = '#';
		long long int e = arrangements(empt, should_nums);
		long long int b = arrangements(brok, should_nums);
		return (e+b);
	}
}


// Ideas for optimization
// implement preemptive pruning e.g. should_nums = 2,5,1 but get_nums(current) = 3,2..
// we prune the current since it will not be able to yield anything
long long int s_arrangements(std::string s, std::vector<long long int> should_nums) {
	long long int res = 0;
	std::string current;
	std::vector<std::string> stack;
	stack.push_back(s);

	while (stack.size() != 0) {
		//std::cout << "Stack size=" << stack.size() << '\n';
		current = stack[stack.size() - 1];
		stack.pop_back();

		if (current.find('?') == std::string::npos) {
			//std::cout << "No question mark\n";
			if (compare_nums(get_nums(current), should_nums) == 1) {
				res += 1;
			}
		} else {
			long long int q_pos = current.find('?');
			std::string empt = current;
			std::string brok = current;
			empt[q_pos] = '.';
			brok[q_pos] = '#';
			stack.push_back(empt);
			stack.push_back(brok);
		}
	}
	return res;
}

long long int fits(std::string& s, long long int group) {
	if (s.size() < group) return 0;
	// Check if there are enough possible #s
	for (long long int idx = 0; idx < group; idx++) {
		if (s[idx] == '.') {
			return 0;
		}
	}
	// Check if there is a separator (? or .)
	if (s[group] == '#') {
		return 0;
	}
	return 1;
}

long long int count_leftmost_hashes(std::string& s) {

	if (s.find('#') == std::string::npos) {
		return 0;
	} else {
		return 1;
	}

	//long long int res = 0;
	//long long int i = 0;
	//while (s[i] != '#'){
	//	i++;
	//}
	//while (s[i] == '#'){
	//	res += 1;
	//	i++;
	//}
	//return res;
}

long long int d_arrangements(std::string s, std::vector<long long int>& rev_should_nums, std::unordered_map<std::string, long long int>& memo){
	long long int res = 0;


	// Discard the dots in the beginning
	// i is treated as the beginning of the group
	long long int i = 0;
	while (s[i] == '.') {
		i++;
	}
	s = s.substr(i);

	if (s == "") {
		if (rev_should_nums.size() == 0) {
			return 1;
		} else {
			return 0;
		}
	}

	if (rev_should_nums.size() == 0) {
		if (s.find("#") == std::string::npos) {
			return 1;
		}
		else {
			return 0;
		}
	}

	std::string mk = s;
	for (auto num : rev_should_nums) mk += num + ',';
	if (memo.find(mk) != memo.end()) {
		return memo[mk];
	}

	long long int min_remaining_size = 0;
	if (rev_should_nums.size() > 1) {
		for (long long int j = 0; j < rev_should_nums.size() - 1; j++) {
			min_remaining_size += rev_should_nums[j] + 1;
		}
		min_remaining_size -= 1;
	}
	min_remaining_size = min_remaining_size < 0 ? 0 : min_remaining_size;

	long long int done = 0;
	char sliding_prefix = '.';
	std::string s_sliding = s;
	std::string s_chomped;
	long long int begin = 0;
	//while (begin < (s.size() - min_remaining_size) && (done != 1)) {
	while (begin < s.size() && (done != 1)) {
		if (begin < s.size()) {
			s_sliding = s.substr(begin);
			std::string s_passed = s.substr(0, begin);
			if (count_leftmost_hashes(s_passed) > 0) {
				done = 1;
			}
		} else {
			break;
		}

		if (0 < begin) {
			sliding_prefix = s[begin-1];
		} else {
			sliding_prefix = '.';
		}

		if ((fits(s_sliding, rev_should_nums[rev_should_nums.size() - 1]) == 1) && (s_sliding.size() != 0) && sliding_prefix != '#') {
			long long int group = rev_should_nums[rev_should_nums.size() - 1];
			rev_should_nums.pop_back();

			// Copy of the string with the group removed
			if (group+1 < s_sliding.size()) {
				s_chomped = s_sliding.substr(group + 1); // accounting for . separator
			} else {
				s_chomped = "";
			}

			res += d_arrangements(s_chomped, rev_should_nums, memo);
			rev_should_nums.push_back(group);
		}
		begin++;
		//std::cout << "Trying to call s.substr: s=" << s << " and begin=" << begin << '\n';
	}
	memo[mk] = res;
	return res;
}

int main() {

	std::ifstream f("input.txt");
	//std::ifstream f("example.txt");

	if (!f.is_open()) {
		std::cout << "Couldnt open the file" << '\n';
	}

	std::vector<std::string> lines;
	std::vector<std::vector<long long int>> global_nums;

	std::string line;
	while (std::getline(f, line)) {
		long long int space_idx = line.find(' ');
		std::string left_side = line.substr(0, space_idx);
		std::string right_side = line.substr(space_idx + 1);
		// Preprocessing should be here
		// reduce all dots to a single one
		std::string repeated_left_side = left_side;
		repeated_left_side += "?";
		repeated_left_side += left_side;
		repeated_left_side += "?";
		repeated_left_side += left_side;
		repeated_left_side += "?";
		repeated_left_side += left_side;
		repeated_left_side += "?";
		repeated_left_side += left_side;


		//std::cout << "left side repeated is\n";
		//std::cout << repeated_left_side;
		//std::cout << std::endl;
		
		// part 1
		//lines.push_back(left_side);
		// part 2
		lines.push_back(repeated_left_side);

		std::stringstream iss(right_side);

		std::vector<long long int> line_nums;
		long long int number;
		char comma;

		while (iss >> number) {
			line_nums.push_back(number);
			// Check for the comma, but don't require it after the last number
			if (iss >> comma && comma != ',') {
				std::cerr << "Error parsing numbers." << std::endl;
				return 1; // Return an error code
			}
		}

		std::vector<long long int> line_nums_repeated = {};
		for (long long int i = 0; i < line_nums.size()*5; i++) {
			line_nums_repeated.push_back(line_nums[i%line_nums.size()]);
		}

		//std::cout << "Line nums repeated: \n";
		//for (auto n : line_nums_repeated) std::cout << n << " ";
		//std::cout << '\n';

		// part 1
		//global_nums.push_back(line_nums);
		// part 2
		global_nums.push_back(line_nums_repeated);
	}


	//.???#??????#. 6,1
	//std::string s = ".???#??????#.";
	//std::vector<long long int> should_nums = {6,1};
	//std::vector<long long int> rev_should_nums = {1,6};

	//long long int cur = arrangements(s, should_nums);
	//long long int cur_s = s_arrangements(s, should_nums);
	//long long int cur_opt = d_arrangements(s, rev_should_nums);
	//std::cout << "arrangements=" << cur << '\n';
	//std::cout << "s_arrangements=" << cur_s << '\n';
	//std::cout << "optimized=" << cur_opt << '\n';
	
	long long int res = 0;
	long long int res_opt = 0;
	//for (long long int i = 1; i < 2; i++) {
	for (long long int i = 0; i < global_nums.size(); i++) {
		std::vector<long long int> should_nums = global_nums[i];
		std::string s = lines[i];

		long long int cur = 0;
		long long int cur_opt = 0;

		//cur = arrangements(s, should_nums);
		//res += cur;
		//std::cout << "Possible arrangements (unoptim) =" << cur << '\n';

		std::unordered_map<std::string, long long int> memo;
		std::reverse(should_nums.begin(), should_nums.end());
		cur_opt = d_arrangements(s, should_nums, memo);
		res_opt += cur_opt;
		std::cout << "Possible arrangements (optimized) =" << cur_opt << '\n';

		//if (cur != cur_opt) {
		//	std::cout << "Results for i=" << i << '\n';
		//	std::cout << "line=" << s << '\n';
		//	for (auto num : should_nums) std::cout << num << '-';
		//	std::cout << '\n';
		//	std::cout << "Possible arrangements (unoptim) =" << cur << '\n';
		//	std::cout << "Possible arrangements (optimized) =" << cur_opt << '\n';
		//}
	}

	// not 1264298909
	std::cout << "unoptimized " << res << '\n';
	std::cout << "The answer is " << res_opt << '\n';

	f.close();
	
	return 0;
}
