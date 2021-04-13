#include <cmath>

#include "im2gr/index.h"

Index::Index(int x, int y, int z) : _x(x), _y(y), _z(z) {
  // empty
}

Index::Index(int *idx) {
  _x = idx[0];
  _y = idx[1];
  _z = idx[2];
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

int *Index::asptr() const {
  int *ret = new int[3];
  ret[0] = _x;
  ret[1] = _y;
  ret[2] = _z;

  return ret;
}

float distance(const Index &idx1, const Index &idx2) {
  float dp = (float) (
    pow(idx1._x-idx2._x, 2) + 
    pow(idx1._y-idx2._y, 2) + 
    pow(idx1._z-idx2._z, 2)
  );
  return (float) sqrt(dp);
}

Index max(const Index &idx1, const Index &idx2) {
  int *m = new int[3];
  int *one = idx1.asptr();
  int *two = idx2.asptr();

  for (short i = 0; i < 3; i++)
    m[i] = one[i] > two[i] ? one[i] : two[i];

  auto ret = Index(m);

  delete[] m;
  delete[] one;
  delete[] two;

  return ret;
}

Index min(const Index &idx1, const Index &idx2) {
  int *m = new int[3];
  int *one = idx1.asptr();
  int *two = idx2.asptr();

  for (short i = 0; i < 3; i++)
    m[i] = one[i] < two[i] ? one[i] : two[i];

  auto ret = Index(m);

  delete[] m;
  delete[] one;
  delete[] two;

  return ret;
}

Index operator+(const Index &idx1, const Index &idx2) {
  Index ret;
  ret._x = idx1._x + idx2._x;
  ret._y = idx1._y + idx2._y;
  ret._z = idx1._z + idx2._z;
  return ret;
}

Index operator-(const Index &idx1, const Index &idx2) {
  Index ret;
  ret._x = idx1._x - idx2._x;
  ret._y = idx1._y - idx2._y;
  ret._z = idx1._z - idx2._z;
  return ret;
}

bool operator==(const Index &idx1, const Index &idx2) {
  return ((idx1._x == idx2._x) && (idx1._y == idx2._y) && (idx1._z == idx2._z));
}

std::ostream &operator<<(std::ostream &o, const Index &idx) {
  o << "[" << idx._x << " " << idx._y << " " << idx._z << "]";
  return o;
}

