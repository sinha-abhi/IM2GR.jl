#ifndef _INDEX_H_
#define _INDEX_H_

#include <ostream>

class Index {
public:
  Index(int x = 0, int y = 0, int z = 0);
  Index(int *idx);

  int x();
  int y();
  int z();

  void set_x(int x);
  void set_y(int y);
  void set_z(int z);

  int *asptr() const;

  friend float distance(const Index &idx1, const Index &idx2);
  friend Index max(const Index &idx1, const Index &idx2);
  friend Index min(const Index &idx1, const Index &idx2);
  friend Index operator+(const Index &idx1, const Index &idx2);
  friend Index operator-(const Index &idx1, const Index &idx2);
  friend bool operator==(const Index &idx1, const Index &idx2);
  friend std::ostream& operator<<(std::ostream &o, const Index &idx);
private:
  int _x, _y, _z;
};

#endif /* _INDEX_H_ */
