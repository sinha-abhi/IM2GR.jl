#ifndef _UTILS_H_
#define _UTILS_H_

#include <algorithm>
#include <fstream>
#include <iterator>
#include <string>
#include <vector>

template<typename T>
T __stousi(std::string s) {
	return ((T) strtoul(s.c_str(), NULL, 10));
}

template<typename T>
std::vector<std::vector<T>> parse_file(int *sz, std::string f) {
	std::vector<std::vector<T>> nums;

	std::ifstream infile(f);

	std::string sz_line;
	std::getline(infile, sz_line);

	std::string _tmp;
	std::istringstream szss(sz_line);
	for (short i = 0; i < 3 && std::getline(szss, _tmp, ' '); ++i)
		sz[i] = atoi(_tmp.c_str());

	std::string line;
	while (getline(infile, line)) {
		std::istringstream ss(line);

		std::istream_iterator<std::string> begin(ss);
		std::istream_iterator<std::string> end;

		std::vector<std::string> s_nums(begin, end);

		std::vector<T> _nums;
		_nums.resize(s_nums.size());

		std::transform(s_nums.begin(), s_nums.end(), _nums.begin(), __stousi<T>);

		nums.push_back(_nums);
	}

	return nums;
}

template<typename T>
void unflatten(T ***arr, int *sz,
			   std::vector<std::vector<T>> flat) {

	for (unsigned z = 0; z < flat.size(); ++z) {
		std::vector<T> xy = flat.at(z);
		int x = 0;
		int y = 0;
		for (unsigned idx = 0; idx < xy.size(); ++idx) {
			arr[x++][y][z] = xy.at(idx);
			if (x == sz[0]) {
				y++;
				x = 0;
			}
		}
	}
}

#endif /* _UTILS_H_ */
