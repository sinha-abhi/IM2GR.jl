#include <iostream>

#include "index.h"
#include "MRILoader.h"

int main() {
	MRILoader loader("../data/lgemri.txt");

	loader.im2gr(1);

	Index *ei = loader.get_ei();
	std::cout << ei[5] << std::endl;
	std::cout << ei[105] << std::endl;

	float *evi = loader.get_evi();
	std::cout << evi[105] << std::endl;

	return 0;
}
