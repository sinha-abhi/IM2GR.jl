#include <iostream>

#include "../../include/loader.h"

int main() {
	Loader loader("../data/lgemri.txt", 1);

	loader.im2gr();

	return 0;
}
