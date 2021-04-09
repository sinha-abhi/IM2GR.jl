#ifndef _INDEX_H_
#define _INDEX_H_

#include <ostream>
#include <utility>

class Index {
public:
  Index(int x = 0, int y = 0, int z = 0);

  int x();
  int y();
  int z();

  void set_x(int x);
  void set_y(int y);
  void set_z(int z);

  std::pair<Index, Index> ends(const int d, int mx, int my, int mz);

  friend float distance(const Index &idx1, const Index &idx2);
  friend bool operator==(const Index &idx1, const Index &idx2);
  friend std::ostream& operator<<(std::ostream &o, const Index &idx);
private:
  int _x, _y, _z;
};

#endif /* _INDEX_H_ */
