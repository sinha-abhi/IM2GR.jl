#include <iostream>

#include "MRILoader.h"

#define DEBUG 1

static float mri_diff_func(float x, float y) {
    return (std::min(sqrt(x) / 63.0, 1.0) - std::min(sqrt(y) / 63.0, 1.0));
}

MRILoader::MRILoader(const char *f, int dist) {
	sz = new int[3];
	vc = 0;
	d = dist;

	mri = new uint8_t**[MAX_MRI_X];
	for (unsigned i = 0; i < MAX_MRI_X; ++i) {
		mri[i] = new uint8_t*[MAX_MRI_Y];
		for (unsigned j = 0; j < MAX_MRI_Y; ++j)
			mri[i][j] = new uint8_t[MAX_MRI_Z];
	}

	std::string fname(f);
	auto mri_data(parse_file(sz, fname));
#if DEBUG
	std::cout << "Parsed file." << std::endl;
#endif

	unflatten(mri, sz, mri_data);
#if DEBUG
	std::cout << "Expanded MRI." << std::endl;
#endif

	unsigned int upv = graph_vector_ub(sz, d);

	ei = new Index[upv];
	ej = new Index[upv];
	evi = new float[upv];
	evd = new float[upv];
}

MRILoader::~MRILoader() {
	delete[] sz;

	for (int i = 0; i < MAX_MRI_X; ++i) {
		for (int j = 0; j < MAX_MRI_Y; ++j)
			delete[] mri[i][j];
		delete[] mri[i];
	}
	delete[] mri;

	delete[] ei;
	delete[] ej;
	delete[] evi;
	delete[] evd;
}

void MRILoader::im2gr() {
#if DEBUG
	std::cout << "Processing image..." << std::endl;
#endif
    short max_x = sz[0] - 1;
    short max_y = sz[1] - 1;
    short max_z = sz[2] - 1;

    short x, y, z;
    short nx, ny, nz;

    Index src, dest;
    Index idx_low, idx_up;

    std::pair<Index, Index> bounds;

    uint8_t pi, pj;
    float dist, dst_sq;

    vc = 0;
    for (x = 0; x <= max_x; ++x) {
        for (y = 0; y <= max_y; ++y) {
            for (z = 0; z <= max_z; ++z) {
                src.set_x(x);
                src.set_y(y);
                src.set_z(z);

                bounds = src.find(d, max_x, max_y, max_z);
                idx_low = bounds.first;
                idx_up = bounds.second;

                pi = mri[x][y][z];
                for (nx = idx_low.x(); nx <= idx_up.x(); ++nx) {
                    for (ny = idx_low.y(); ny <= idx_up.y(); ++ny) {
                        for (nz = idx_low.z(); nz <= idx_up.z(); ++nz) {
                            dest.set_x(nx);
                            dest.set_y(ny);
                            dest.set_z(nz);
                            if (src == dest)
                                continue;

                            dist = distance(src, dest);
                            dst_sq = dist * dist;
                            pj = mri[nx][ny][nz];

                            ei[vc] = src;
                            ej[vc] = dest;
                            evd[vc] = dst_sq;
                            evi[vc] = mri_diff_func(pi, pj);

                            vc++;
                        }
                    }
                }
            }
        }
    }
#if DEBUG
    std::cout << "Done. vc: " << vc << std::endl;
#endif
}

