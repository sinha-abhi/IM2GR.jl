#include <algorithm>
#include <climits>
#include <cmath>
#include <fstream>
#include <iostream>
#include <iterator>
#include <string>
#include <sstream>
#include <vector>

#include <stdlib.h>

#define DEBUG 0

#define MRI_FILE "data/lgemri.txt"

#define MAX_MRI_X 576
#define MAX_MRI_Y 576
#define MAX_MRI_Z 88

#define MAX_VEC_SZ (751321624 + 1)

class Index {
private:
	short x_val, y_val, z_val;
public:
	Index() {
		x_val = -1;
		y_val = -1;
		z_val = -1;
	}
	Index(short x, short y, short z) {
		x_val = x;
		y_val = y;
		z_val = z;
	}
	short x() { return x_val; }
	short y() { return y_val; }
	short z() { return z_val; }

	void set_x(short x) { x_val = x; }
	void set_y(short y) { y_val = y; }
	void set_z(short z) { z_val = z; }

	friend float distance(const Index &idx1, const Index &idx2);
	friend bool operator==(const Index &idx1, const Index &idx2);
	friend std::ostream &operator<<(std::ostream &o, const Index &idx);
};

bool operator==(const Index &idx1, const Index &idx2) {
	return ((idx1.x_val == idx2.x_val) && (idx1.y_val == idx2.y_val) && (idx1.z_val == idx2.z_val));
}

float distance(const Index &idx1, const Index &idx2) {
	short diff_x = idx1.x_val - idx2.x_val;
	short diff_y = idx1.y_val - idx2.y_val;
	short diff_z = idx1.z_val - idx2.z_val;

	return sqrt(diff_x * diff_x + diff_y * diff_y + diff_z * diff_z);
}

std::ostream &operator<<(std::ostream &o, const Index &idx) {
	o << idx.x_val << " " << idx.y_val << " " << idx.z_val;
	return o;
}

unsigned char __stousi(std::string s) {
	return ((unsigned char) strtoul(s.c_str(), NULL, 10));
}

std::vector<std::vector<unsigned char>> parse_file(int *sz, std::string f) {
	std::vector<std::vector<unsigned char>> nums;

	std::ifstream infile(f);

	std::string sz_line;
	getline(infile, sz_line);

	std::string _tmp;
	std::istringstream szss(sz_line);
	for (short i = 0; i < 3 && getline(szss, _tmp, ' '); ++i)
		sz[i] = atoi(_tmp.c_str());

	std::string line;
	while (getline(infile, line)) {
		std::istringstream ss(line);

		std::istream_iterator<std::string> begin(ss);
		std::istream_iterator<std::string> end;

		std::vector<std::string> s_nums(begin, end);

		std::vector<unsigned char> _nums;
		_nums.resize(s_nums.size());

		std::transform(s_nums.begin(), s_nums.end(), _nums.begin(), __stousi);

		nums.push_back(_nums);
	}

	return nums;
}

void unflatten(unsigned char ***arr, int *sz,
			   std::vector<std::vector<unsigned char>> flat) {

	for (unsigned z = 0; z < flat.size(); ++z) {
		std::vector<unsigned char> xy = flat.at(z);
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

typedef float (*diff_func_call)(float x, float y);

float mri_diff_func(float x, float y) {
	return (std::min(sqrt(x) / 63.0, 1.0) - std::min(sqrt(y) / 63.0, 1.0));
}

/**
 *	im    : image
 *	im_sz : image size (x, y, z)
 *	d 	  : distance of neighbors
 *	diff  : differential function
 */
void im2gr(Index *ei, Index *ej, float *evd, float *evi,
		   unsigned char ***im, int *im_sz, int d,
		   diff_func_call diff) {
	short max_x = im_sz[0] - 1;
	short max_y = im_sz[1] - 1;
	short max_z = im_sz[2] - 1;

	short x, y, z;
	short nx, ny, nz;

	short low_x, low_y, low_z;
	short up_x, up_y, up_z;
	Index src, dest;
	Index idx_low, idx_up;

	unsigned char pi, pj;
	float dist, dst_sq;

	unsigned vc = 0;
	for (x = 0; x <= max_x; ++x) {
#if DEBUG
		std::cout << "x: " << x << std::endl;
#endif
		for (y = 0; y <= max_y; ++y) {
#if DEBUG
			std::cout << "y: " << y << std::endl;
#endif
			for (z = 0; z <= max_z; ++z) {
#if DEBUG
				std::cout << "x: " << x << " y: " << y << " z: " << z << " vc: " << vc << std::endl;
#endif
				low_x = x - d;
				low_y = y - d;
				low_z = z - d;

				if (low_x < 0)
					idx_low.set_x(0);
				else
					idx_low.set_x(low_x);

				if (low_y < 0)
					idx_low.set_y(0);
				else
					idx_low.set_y(low_y);

				if (low_z < 0)
					idx_low.set_z(0);
				else
					idx_low.set_z(low_z);

				up_x = x + d;
				up_y = y + d;
				up_z = z + d;

				if (up_x > max_x)
					idx_up.set_x(max_x);
				else
					idx_up.set_x(up_x);

				if (up_y > max_y)
					idx_up.set_y(max_y);
				else
					idx_up.set_y(up_y);

				if (up_z > max_z)
					idx_up.set_z(max_z);
				else
					idx_up.set_z(up_z);

				src.set_x(x);
				src.set_y(y);
				src.set_z(z);
				pi = im[x][y][z];
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
							pj = im[nx][ny][nz];

							ei[vc]  = src;
							ej[vc]  = dest;
							evd[vc] = dst_sq;
							evi[vc] = diff(pi, pj);

							vc++;
						}
					}
				}
			}
		}
	}
#if DEBUG
	std::cout << "vc: " << vc << std::endl;
#endif
}

int main() {
	int *mri_sz = new int[3];
	std::vector<std::vector<unsigned char>> flat_mri(parse_file(mri_sz, MRI_FILE));

	std::cout << "Parsed file... " << flat_mri.size() << std::endl;

	if (mri_sz[0] > MAX_MRI_X || mri_sz[1] > MAX_MRI_Y || mri_sz[2] > MAX_MRI_Z) {
		std::cerr << "MRI dimensions exceed limit:" << std::endl;
		std::cerr << "\t x = " << mri_sz[0] << ", max = " << MAX_MRI_X << std::endl;
		std::cerr << "\t y = " << mri_sz[1] << ", max = " << MAX_MRI_Y << std::endl;
		std::cerr << "\t z = " << mri_sz[2] << ", max = " << MAX_MRI_Z << std::endl;

		return -1;
	}

	// allocate memory for mri image
	unsigned char ***mri = new unsigned char**[MAX_MRI_X];
	for (int i = 0; i < MAX_MRI_X; ++i) {
		mri[i] = new unsigned char*[MAX_MRI_Y];
		for (int j = 0; j < MAX_MRI_Y; ++j)
			mri[i][j] = new unsigned char[MAX_MRI_Z];
	}

#if DEBUG
		// fill a known pattern to check alignment of parsing: 0 ... 575
		for (int i = 0; i < MAX_MRI_Z; i++) {
			int val = 0;
			for (int j = 0; j < MAX_MRI_Y * MAX_MRI_X; j++) {
				flat_mri.at(i).at(j) = val++;
				if (val == MAX_MRI_X) {
					val = 0;
				}
			}
		}
#endif

	unflatten(mri, mri_sz, flat_mri);

	std::cout << "Unflattened vector. Processing image..." << std::endl;

	Index *mri_ei = new Index[MAX_VEC_SZ];
	Index *mri_ej = new Index[MAX_VEC_SZ];
	float *mri_evi = new float[MAX_VEC_SZ];
	float *mri_evd = new float[MAX_VEC_SZ];
	im2gr(mri_ei, mri_ej, mri_evi, mri_evd, mri, mri_sz, 1, mri_diff_func);

	std::cout << "Done." << std::endl;

#if DEBUG
		// find max val (should be 254)
		unsigned char max = 0;
		for (const std::vector<unsigned char> &v : flat_mri) {
			for (unsigned char i : v) {
				if (i > max) {
					max = i;
				}
			}
		}
		std::cout << max << std::endl;
#endif

#if DEBUG
		// print MRI data
		for (const std::vector<unsigned char> &v : flat_mri) {
			for (unsigned char i : v)
				std::cout << i << ' ';

			std::cout << std::endl;
		}
#endif

	// free memory
	delete[] mri_sz;

	for (int i = 0; i < MAX_MRI_X; ++i) {
		for (int j = 0; j < MAX_MRI_Y; ++j)
			delete[] mri[i][j];

		delete[] mri[i];
	}
	delete[] mri;

	delete[] mri_ei;
	delete[] mri_ej;
	delete[] mri_evi;
	delete[] mri_evd;

	return 0;
}

