#ifndef _UTILS_H_
#define _UTILS_H_

#include <algorithm>
#include <cstdint>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

uint8_t __stousi(std::string s);
std::vector<std::vector<uint8_t>> parse_file(int *sz, std::string f);
void unflatten(uint8_t ***arr, int *sz, std::vector<std::vector<uint8_t>> data);
void flatten(uint8_t *arr, std::vector<std::vector<uint8_t>> data);

#endif /* _UTILS_H_ */
