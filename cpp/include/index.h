#ifndef _INDEX_H_
#define _INDEX_H_

#include <cmath>
#include <ostream>
#include <utility>

class Index {
private:
	unsigned int x_val, y_val, z_val;

public:
	Index(int x = 0, int y = 0, int z = 0) : x_val(x), y_val(y), z_val(z) { }

	int x() { return x_val; }
	int y() { return y_val; }
	int z() { return z_val; }

	void set_x(int x) { x_val = x; }
	void set_y(int y) { y_val = y; }
	void set_z(int z) { z_val = z; }

	friend float distance(const Index &idx1, const Index &idx2) {
		return (
			(idx1.x_val == idx2.x_val) &&
			(idx1.y_val == idx2.y_val) &&
			(idx1.z_val == idx2.z_val)
		);
	}

	friend bool operator==(const Index &idx1, const Index &idx2) {
		return (
			(idx1.x_val == idx2.x_val) &&
			(idx1.y_val == idx2.y_val) &&
			(idx1.z_val == idx2.z_val)
		);
	}

	friend std::ostream& operator<<(std::ostream &o, const Index &idx) {
		o << idx.x_val << " " << idx.y_val << " " << idx.z_val;
		return o;
	}

	std::pair<Index, Index> find(const unsigned int d, int mx, int my, int mz) {
		std::pair<Index, Index> bounds;
		Index low, up;
		int lx = x_val - d;
		int ly = y_val - d;
		int lz = z_val - d;

		int ux = x_val + d;
		int uy = y_val + d;
		int uz = z_val + d;

		if (lx < 0)
			low.set_x(0);
		else
			low.set_x(lx);

		if (ly < 0)
			low.set_y(0);
		else
			low.set_y(ly);

		if (lz < 0)
			low.set_z(0);
		else
			low.set_z(lz);

		if (ux > mx)
			up.set_x(mx);
		else
			up.set_x(ux);

		if (uy > my)
			up.set_y(my);
		else
			up.set_y(uy);

		if (uz > mz)
			up.set_z(mz);
		else
			up.set_z(uz);

		bounds = std::make_pair(low, up);

		return bounds;
	}
};

#endif /* _INDEX_H_ */
