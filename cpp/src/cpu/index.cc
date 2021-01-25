#include <cmath>

#include "index.h"

bool operator==(const Index &idx1, const Index &idx2) {
	return (
		(idx1.x_val == idx2.x_val) &&
		(idx1.y_val == idx2.y_val) &&
		(idx1.z_val == idx2.z_val)
	);
}

float distance(const Index &idx1, const Index &idx2) {
	short diff_x = idx1.x_val - idx2.x_val;
	short diff_y = idx1.y_val - idx2.y_val;
	short diff_z = idx1.z_val - idx2.z_val;

	return sqrt(diff_x * diff_x + diff_y * diff_y + diff_z * diff_z);
}

std::ostream& operator<<(std::ostream &o, const Index &idx) {
	o << idx.x_val << " " << idx.y_val << " " << idx.z_val;
	return o;
}
