#include <fstream>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <vector>
#include <string>

int hash(std::string& s) {
	int current_value = 0;
	for (char c : s) {
		current_value += (c);
		current_value *= 17;
		current_value %= 256;
	}
	return current_value;
}

int main() {

	std::ifstream f("input.txt");

	if (!f.is_open()) {
		std::cout << "Couldn't open the file\n";
	}

	std::string line;

	std::getline(f, line);

	std::vector<std::string> steps;
	std::string step;

	// Parsing the input into a vector of steps
	for (int i = 0; i < line.size(); i++) {
		if (line[i] != ',') {
			step += line[i];
		} else {
			steps.push_back(step);
			step = "";
		}
	}
	if (!step.empty()) steps.push_back(step);

	std::vector<std::vector<std::string>> hashtable(256);

	// Part 2
	int res2 = 0;
	for (auto& s : steps) {
		std::cout << "Current step is " << s << '\n';

		std::string label = "";
		int idx = 0;
		while (s[idx] != '=' && s[idx] != '-') {
			label += s[idx];
			idx++;
		}
		int key = hash(label);
		if (s[idx] == '=') {
			// Turning the char into a num
			int num = (s[idx+1] - 48);
			auto bucket = hashtable.at(key);

			bool found = false;
			for (int i = 0; i < bucket.size(); i++) {
				if (bucket.at(i).find(label) != std::string::npos) {
					bucket.at(i) = label + " " + s[idx+1];
					found = true;
					hashtable.at(key) = bucket;
				}
			}
			if (!found) {
				bucket.push_back(label + " " + s[idx+1]);
				hashtable.at(key) = bucket;
			}

		} else if (s[idx] == '-') {
			auto bucket = hashtable.at(key);
			bucket.erase(std::remove_if(bucket.begin(), bucket.end(), [label](std::string& s){return s.find(label) != std::string::npos;}),
				bucket.end());
			hashtable.at(key) = bucket;
		}
	}

	for (int i = 0; i < hashtable.size(); i++) {
		for (int j = 0; j < hashtable.at(i).size(); j++) {
      			std::string record = hashtable.at(i).at(j);
			std::cout << record << " is the record at i=" << i << ", j=" << j << '\n';
			std::cout << "" << (i+1) << "*" << (j+1) << "*" << record.at(record.size() - 1) << '\n';
			res2 += (i+1) * (j+1) * (record.at(record.size() - 1) - '0');
		}
	}

	// Part 1
	int res1 = 0;
	for (auto s : steps) {
		res1 += hash(s);
		//std::cout << s << '=' << hash(s) << '\n';
	}

	std::cout << "The answer for part 1 is " << res1 << '\n';
	std::cout << "The answer for part 2 is " << res2 << '\n';

	return 0;
}
