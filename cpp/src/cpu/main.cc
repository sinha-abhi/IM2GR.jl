#include <iostream>

#include "loader.h"

static float mri_diff_func(float x, float y) {
    return (std::min(sqrt(x) / 63.0, 1.0) - std::min(sqrt(y) / 63.0, 1.0));
}

int main(int argc, char **argv) {
	int d = 1;
	if (argc < 2)
		std::cerr << "Defaulting to d = " << d << std::endl;
	else
		d = atoi(argv[1]);

    Loader<short> loader("../data/lgemri.txt", d);
    loader.im2gr(mri_diff_func);

    return 0;
}
