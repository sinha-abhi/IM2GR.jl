#include <iostream>

#include "im2gr/diff-func.h"
#include "im2gr/index.h"
#include "im2gr/im2gr.h"
#include "im2gr/bounds.h"

Image st_construct(Data *data, const int d, diff_fn diff) {
#if DEBUG
  std::cout << "Constructing image... SingleThread" << std::endl;
#endif

  auto _data = data->get_data();
  auto sz = data->get_size();

  auto ub = fullbound(sz, d);
#if DEBUG
  std::cout << "Vector upperbound: " << ub << std::endl;
#endif

  Index *ei = new Index[ub];
  Index *ej = new Index[ub];
  float *evd = new float[ub];
  float *evi = new float[ub];

  int max_x = sz[0] - 1;
  int max_y = sz[1] - 1;
  int max_z = sz[2] - 1;

  Index src, dest;
  Index idx_low, idx_up;

  std::pair<Index, Index> bounds;
  int x, y, z;
  int nx, ny, nz;
  size_t vc = 0;
  uint8_t pi, pj;
  float dist, dst_sq;
  for (x = 0; x <= max_x; ++x) {
    for (y = 0; y <= max_y; ++y) {
      for (z = 0; z <= max_z; ++z) {
        src.set_x(x);
        src.set_y(y);
        src.set_z(z);

        bounds = src.ends(d, max_x, max_y, max_z);
        idx_low = bounds.first;
        idx_up = bounds.second;

        pi = _data[x][y][z];
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
              pj = _data[nx][ny][nz];

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

  return Image(ei, ej, evd, evi, vc, d);
}

Image mt_construct(Data *data, const int d, diff_fn diff) {
  // TODO
  return Image(nullptr, nullptr, nullptr, nullptr, 0, 0);
}

Image im2gr(Data *data, const int d, ConstructionMode mode, diff_fn diff) {
  if (mode == SingleThread)
    return st_construct(data, d, diff);
  else if (mode == MultiThread)
    return mt_construct(data, d, diff);
  else
    exit(EXIT_FAILURE);
}

