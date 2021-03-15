#ifndef _INDEX_H_
#define _INDEX_H_

#include <cmath>
#include <ostream>
#include <utility>

template <typename T>
class Index {
private:
    T x_val, y_val, z_val;

public:
    Index(T x = 0, T y = 0, T z = 0) : x_val(x), y_val(y), z_val(z) { }

    int x() { return x_val; }
    int y() { return y_val; }
    int z() { return z_val; }

    void set_x(T x) { x_val = x; }
    void set_y(T y) { y_val = y; }
    void set_z(T z) { z_val = z; }

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
        o << "[" << idx.x_val << " " << idx.y_val << " " << idx.z_val << "]";
        return o;
    }

    std::pair<Index<T>, Index<T>> find(const unsigned int d, T mx, T my, T mz) {
        std::pair<Index<T>, Index<T>> bounds;
        Index low, up;
        T lx = x_val - d;
        T ly = y_val - d;
        T lz = z_val - d;

        T ux = x_val + d;
        T uy = y_val + d;
        T uz = z_val + d;

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
