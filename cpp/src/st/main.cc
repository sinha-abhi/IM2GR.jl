#include <iostream>

#include "MRILoader.h"

int main() {
	MRILoader loader("../data/lgemri.txt", 1);

	loader.im2gr();

	Index *ei = loader.get_ei();
	std::cout << ei[5] << std::endl;
	std::cout << ei[105] << std::endl;

	float *evi = loader.get_evi();
	std::cout << evi[105] << std::endl;

	return 0;
}
