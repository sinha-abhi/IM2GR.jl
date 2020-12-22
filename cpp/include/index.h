#ifndef _INDEX_H_
#define _INDEX_H_

#include <sstream>

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
	friend std::ostream& operator<<(std::ostream &o, const Index &idx);
};

#endif /* _INDEX_H_ */
