#include <iostream>

#include "loader.h"

template <typename T>
Loader<T>::Loader(const char *f, int dist) : d(dist) {
  sz = new int[3];
  vc = 0;

  data = new uint8_t**[MAX_X];
  for (unsigned i = 0; i < MAX_X; ++i) {
    data[i] = new uint8_t*[MAX_Y];
    for (unsigned j = 0; j < MAX_Y; ++j)
      data[i][j] = new uint8_t[MAX_Z];
  }

  std::string fname(f);
  auto _data(parse_file(sz, fname));
#ifdef DEBUG
  std::cout << "Parsed file." << std::endl;
#endif

  unflatten(data, sz, _data);
#ifdef DEBUG
  std::cout << "Expanded Image." << std::endl;
#endif

  unsigned long upv = graph_vector_ub(sz, d);

  ei = new Index<T>[upv];
  ej = new Index<T>[upv];
  evi = new float[upv];
  evd = new float[upv];

#ifdef DEBUG
  std::cout << "Image size: " << sz[0] << ", "
            << sz[1] << ", "
            << sz[2] << std::endl;
  std::cout << "Search distance d: " << d << std::endl;
  std::cout << "Result vector upperbound: " << upv << std::endl;
#endif
}

template <typename T>
Loader<T>::~Loader() {
  delete[] sz;

  for (unsigned i = 0; i < MAX_X; ++i) {
    for (unsigned j = 0; j < MAX_Y; ++j)
      delete[] data[i][j];
    delete[] data[i];
  }
  delete[] data;

  delete[] ei;
  delete[] ej;
  delete[] evi;
  delete[] evd;
}

template <typename T>
void Loader<T>::_find_nghbrs(diff_fn diff, Index<T> begin, Index<T> end) {
  T _x = begin.x();
  T _y = begin.y();
  T _z = begin.z();

  T max_x = end.x();
  T max_y = end.y();
  T max_z = end.z();

  Index<T> src, dest;
  Index<T> idx_low, idx_up;

  std::pair<Index<T>, Index<T>> bounds;

  T x, y, z;
  T nx, ny, nz;
  uint8_t pi, pj;
  float dist, dst_sq;
  for (x = _x; x <= max_x; ++x) {
    for (y = _y; y <= max_y; ++y) {
      for (z = _z; z <= max_z; ++z) {
        src.set_x(x);
        src.set_y(y);
        src.set_z(z);

        bounds = src.find(d, max_x, max_y, max_z);
        idx_low = bounds.first;
        idx_up = bounds.second;

        pi = data[x][y][z];
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
              pj = data[nx][ny][nz];

              // TODO: locking
              ei[vc] = src;
              ej[vc] = dest;
              evd[vc] = dst_sq;
              evi[vc] = diff(pi, pj);

              vc++;
            }
          }
        }
      }
    }
  }
#ifdef DEBUG
  std::cout << "Done. Actual vector size: " << vc << std::endl;
#endif
}

template <typename T>
void Loader<T>::im2gr(diff_fn diff) {
  vc = 0;

#ifdef MULTITHREAD
#ifdef DEBUG
  std::cout << "Processing image... MULTITHREAD" << std::endl;
#endif
  // TODO: divide into blocks and assign work to threads
#else
#ifdef DEBUG
  std::cout << "Processing image... SINGLETHREAD" << std::endl;
#endif
  Index<T> begin;
  Index<T> end(sz[0]-1, sz[1]-1, sz[2]-1);

  _find_nghbrs(diff, begin, end);
#endif
}

template class Loader<short>;
