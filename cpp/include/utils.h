#ifndef _UTILS_H_
#define _UTILS_H_

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

inline uint8_t __stousi(std::string s) {
    return ((uint8_t) strtoul(s.c_str(), NULL, 10));
}

inline std::vector<std::vector<uint8_t>> parse_file(int *sz, std::string f) {
    std::vector<std::vector<uint8_t>> nums;

    std::ifstream infile(f);
    if (!infile.is_open()) {
        std::cerr << "Could not open file: " << f << std::endl;
        return nums;
    }

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

        std::vector<uint8_t> _nums;
        _nums.resize(s_nums.size());

        std::transform(s_nums.begin(), s_nums.end(),
                       _nums.begin(), __stousi);

        nums.push_back(_nums);
    }

    return nums;
}

inline void unflatten(uint8_t ***arr, int *sz, std::vector<std::vector<uint8_t>> data) {
    for (unsigned z = 0; z < data.size(); ++z) {
        std::vector<uint8_t> xy = data.at(z);
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

inline void flatten(uint8_t *arr, std::vector<std::vector<uint8_t>> data) {
    std::vector<uint8_t> flat;
    for (std::vector<uint8_t> &v : data)
        flat.insert(flat.end(), v.begin(), v.end());

    std::copy(flat.begin(), flat.end(), arr);
}

inline unsigned long graph_vector_ub(int *sz, int d) {
    unsigned long ub;
    unsigned int n;
    int x = sz[0];
    int y = sz[1];
    int z = sz[2];

    n = (x - 2) * (y - 2) * (z - 2);
    ub = n * (pow(2 * d + 1, 3) - 1);

    n = 2 * ((x - 2) * (y - 2) + (x - 2) * (z - 2) + (y - 2) * (z - 2));
    ub += n * ((d + 1) * pow(2 * d + 1, 2) - 1);

    n = 4 * (x + y) + 4 * z - 8;
    ub += n * ((2 * d + 1) * pow (d + 1, 2) - 1);

    ub += 8 * (pow(d + 1, 3) - 1);

    return ub;
}

#endif /* _UTILS_H_ */
