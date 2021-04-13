#include <iostream>

#if MULTITHREAD
#include <thread>
#endif

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

#if MULTITHREAD
Image mt_construct(Data *data, const int d, diff_fn diff) {
#if DEBUG
  std::cout << "Constructing image... MultiThread" << std::endl;
#endif
  uint8_t ***_data = data->get_data();
  size_t *sz = data->get_size();

  Index cf;
  Index cl(sz[0]-1, sz[1]-1, sz[2]-1);
  Index dd(d, d, d);
  // find block size
  auto nt = std::thread::hardware_concurrency();
  nt = (nt == 0) ? 2 : nt;
#if DEBUG
  std::cout << "Number of threads: " << nt << std::endl;
#endif
  size_t ml = 0;
  short ax;
  for (ax = 0; ax < 3; ++ax)
    if (sz[ax] > ml)
      ml = sz[ax];
  size_t bl = ml < nt ? ml : (size_t) ceil(ml / nt);
  int nb = (int) ceil(ml / bl);

  // allocate memory for thread local states
  size_t *vcs = new size_t[nb];
  Index *bstarts = new Index[nb];
  Index *bstops = new Index[nb];
  Index *dstarts = new Index[nb];
  Index *dstops = new Index[nb];

  int **eis = new int*[nb];
  int **ejs = new int*[nb];
  float **evds = new float*[nb];
  float **evis = new float*[nb];

  auto sb = sidebound(sz, bl, ax, d);
  auto mb = middlebound(sz, bl, ax, d);

  short _ax;
  int start, stop;
  int *bstart = new int[3];
  int *bstop = new int[3];
  for (int b = 0; b < nb; ++b) {
    start = bl * b;
    stop = start + bl;
    stop = stop > sz[ax]-1 ? sz[ax]-1 : stop;
    for (_ax = 0; _ax < 3; _ax++) {
      bstart[_ax] = (_ax == ax) ? start : 1;
      bstop[_ax] = (_ax == ax) ? stop : sz[_ax]-1;
    }

    bstarts[b] = Index(bstart);
    bstops[b] = Index(bstop);
    dstarts[b] = max(cf, bstarts[b] - dd);
    dstops[b] = min(cl, bstops[b] + dd);

    if ((b == 0) || (b == nb-1)) {
      eis[b] = new int[sb];
      ejs[b] = new int[sb];
      evds[b] = new float[sb];
      evis[b] = new float[sb];
    } else {
      eis[b] = new int[mb];
      ejs[b] = new int[mb];
      evds[b] = new float[mb];
      evis[b] = new float[mb];
    }
  }

  // TODO: spawn threads, combine results

  return Image(nullptr, nullptr, nullptr, nullptr, 0, 0);
}
#endif

Image im2gr(Data *data, const int d, ConstructionMode mode, diff_fn diff) {
#if MULTITHREAD
  if (mode == SingleThread) {
    return st_construct(data, d, diff);
  } else if (mode == MultiThread) {
    return mt_construct(data, d, diff);
  } else {
    std::cerr << "Unsupported construction mode." << std::endl;
    goto err;
  }
#else
  if (mode == SingleThread) {
    return st_construct(data, d, diff);
  } else if (mode == MultiThread) {
    std::cerr << "im2gr was not compiled with the MULTITHREAD flag enabled."
              << std::endl;
    goto err;
  } else {
    std::cerr << "Unsupported construction mode." << std::endl;
    goto err;
  }
#endif
err:
  std::cerr << "im2gr errored while trying to construct graph" << std::endl;
  exit(EXIT_FAILURE);
}

