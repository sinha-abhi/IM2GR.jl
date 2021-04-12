#ifndef _BOUNDS_H_
#define _BOUNDS_H_

#include <algorithm>
#include <cmath>

inline unsigned long fullbound(size_t *sz, const int d) {
  unsigned long ub;
  unsigned long n;
  size_t x = sz[0];
  size_t y = sz[1];
  size_t z = sz[2];

  n = (x-2) * (y-2) * (z-2);
  ub = n * (pow(2*d+1, 3) - 1);

  n = 2 * ((x-2)*(y-2) + (x-2)*(z-2) + (y-2)*(z-2));
  ub += n * ((d+1) * pow(2*d+1, 2) - 1);

  n = 4*(x+y) + 4*z - 8;
  ub += n * ((2*d+1) * pow(d+1, 2) - 1);

  ub += 8 * (pow(d+1, 3) - 1);

  return ub;
}

inline unsigned long planebound(size_t *psz, int d) {
  unsigned long n, ub;

  size_t x = psz[0];
  size_t y = psz[1];
  
  n = (x-2) * (y-2);
  ub = n * d * pow(2*d+1, 2);

  n = 2 * (x+y) - 4;
  ub += n * d * (d+1) * (2*d+1);

  ub += 4 * d * pow(d+1, 2);

  return ub;
}

inline unsigned long middlebound(size_t *sz, int bl, int ax, int d) {
  size_t *bsz = new size_t[3];
  std::copy(sz, sz + 3, bsz);
  bsz[ax] = bl;
  auto fb = fullbound(bsz, d);

  size_t *psz = new size_t[2];
  short a = 0;
  for (short i = 0; i < 3; ++i) {
    if (i == ax)
      continue;
    psz[a++] = bsz[i];
  }

  auto pb = planebound(psz, d);

  delete[] bsz;
  delete[] psz;
  
  return (fb + 2*pb);
}

inline unsigned long sidebound(size_t *sz, int bl, int ax, int d) {
  size_t *bsz = new size_t[3];
  std::copy(sz, sz + 3, bsz);
  bsz[ax] = bl;
  auto fb = fullbound(bsz, d);

  size_t *psz = new size_t[2];
  short a = 0;
  for (short i = 0; i < 3; ++i) {
    if (i == ax)
      continue;
    psz[a++] = bsz[i];
  }

  auto pb = planebound(psz, d);

  delete[] bsz;
  delete[] psz;
  
  return (fb+pb);
}

#endif /* _BOUNDS_H_ */
