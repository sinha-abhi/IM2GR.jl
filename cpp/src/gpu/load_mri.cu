#include <iostream>

#include "cuda_utils.h"
#include "utils.h"

#define MRI_FILE "../data/lgemri.txt"

__global__ void traverse() {

}

int main() {
	int *sz = new int[3];

	std::string fname(MRI_FILE);
	std::vector<std::vector<uint8_t>> mri_data(parse_file(sz, fname));

	int ne = sz[0] * sz[1] * sz[2]; // number of elements
	uint8_t *mri = (uint8_t *) malloc(ne * sizeof(uint8_t));
	flatten(mri, mri_data);

    return 0;
}
