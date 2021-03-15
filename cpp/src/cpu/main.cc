#include <iostream>

#include "loader.h"

int main() {
	Index<uint16_t> test;
	std::cout << "index: " << sizeof(test) << std::endl;

    Loader<short> loader("../data/lgemri.txt", 1);

    loader.im2gr();

    return 0;
}
