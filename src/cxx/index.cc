#include <cmath>

#include "im2gr/index.h"

Index::Index(int x, int y, int z) : _x(x), _y(y), _z(z) {
  // empty
}

int Index::x() {
  return _x;
}

int Index::y() {
  return _y;
}

int Index::z() {
  return _z;
}

void Index::set_x(int x) {
  _x = x;
}

void Index::set_y(int y) {
  _y = y;
}

void Index::set_z(int z) {
  _z = z;
}

std::pair<Index, Index> Index::ends(const int d, int mx, int my, int mz) {
  Index low, up;
  int lx = _x - d;
  int ly = _y - d;
  int lz = _z - d;

  int ux = _x + d;
  int uy = _y + d;
  int uz = _z + d;

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

  return std::make_pair(low, up);
}

float distance(const Index &i1, const Index &i2) {
  float dp = (float) (
    pow(i1._x-i2._x, 2) + 
    pow(i1._y-i2._y, 2) + 
    pow(i1._z-i2._z, 2)
  );
  return (float) sqrt(dp);
}

bool operator==(const Index &i1, const Index &i2) {
  return ((i1._x == i2._x) && (i1._y == i2._y) && (i1._z == i2._z));
}

std::ostream &operator<<(std::ostream &o, const Index &idx) {
  o << "[" << idx._x << " " << idx._y << " " << idx._z << "]";
  return o;
}
