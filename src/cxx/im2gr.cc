#include <iostream>

#if MULTITHREAD
#include <cstring>
#include <numeric>
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

  Index cf;
  Index cl(max_x, max_y, max_z);
  Index dd(d, d, d);

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

        idx_low = max(cf, src-dd);
        idx_up = min(cl, src+dd);

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
void mt_construct_kernel(uint8_t ***&data, Index &dd, 
    diff_fn &diff, Index *&ei, Index *&ej, float *&evd, float *&evi,
    Index &bstart, Index &bstop, Index &dstart, Index &dstop, size_t &vc) {
  int sx = bstart.x();
  int sy = bstart.y();
  int sz = bstart.z();
  int mx = bstop.x();
  int my = bstop.y();
  int mz = bstop.z();

  vc = 0;
  Index src, dest;
  Index idx_low, idx_up;

  int x, y, z;
  int nx, ny, nz;
  uint8_t pi, pj;
  float dist, dst_sq;
  for (x = sx; x <= mx; ++x) {
    for (y = sy; y <= my; ++y) {
      for (z = sz; z <= mz; ++z) {
        src.set_x(x);
        src.set_y(y);
        src.set_z(z);

        idx_low = max(dstart, src-dd);
        idx_up = min(dstop, src+dd);

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
}

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
  for (short a = 0; a < 3; ++a) {
    if (sz[a] > ml) {
      ml = sz[a];
      ax = a;
    }
  }

  size_t bl = ml < nt ? ml : (size_t) ceil(ml / nt);
  int nb = (int) ceil(ml / bl);

  // allocate memory for thread local states
  size_t *vcs = new size_t[nb];
  Index *bstarts = new Index[nb];
  Index *bstops = new Index[nb];
  Index *dstarts = new Index[nb];
  Index *dstops = new Index[nb];

  Index **eis = new Index*[nb];
  Index **ejs = new Index*[nb];
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
      bstart[_ax] = (_ax == ax) ? start : 0;
      bstop[_ax] = (_ax == ax) ? stop : sz[_ax]-1;
    }

    bstarts[b] = Index(bstart);
    bstops[b] = Index(bstop);
    dstarts[b] = max(cf, bstarts[b] - dd);
    dstops[b] = min(cl, bstops[b] + dd);

    if ((b == 0) || (b == nb-1)) {
      eis[b] = new Index[sb];
      ejs[b] = new Index[sb];
      evds[b] = new float[sb];
      evis[b] = new float[sb];
    } else {
      eis[b] = new Index[mb];
      ejs[b] = new Index[mb];
      evds[b] = new float[mb];
      evis[b] = new float[mb];
    }
  }

  std::vector<std::thread> pool(nb);
  for (int b = 0; b < nb; ++b) {
    pool[b] = std::thread(mt_construct_kernel, std::ref(_data), 
        std::ref(dd), std::ref(diff),
        std::ref(eis[b]), std::ref(ejs[b]), std::ref(evds[b]), std::ref(evis[b]),
        std::ref(bstarts[b]), std::ref(bstops[b]), 
        std::ref(dstarts[b]), std::ref(dstops[b]), std::ref(vcs[b]));
  }

  for (auto &t : pool)
    t.join();

  // combine results with memcpy
  size_t _vc = 0;
  auto vc = std::accumulate(vcs, vcs+nb, 0);
  Index *ei = new Index[vc];
  Index *ej = new Index[vc];
  float *evd = new float[vc];
  float *evi = new float[vc];
  for (int b = 0; b < nb; ++b) {
    memcpy(ei+_vc, eis[b], vcs[b]*sizeof(Index));
    memcpy(ej+_vc, eis[b], vcs[b]*sizeof(Index));
    memcpy(evd+_vc, eis[b], vcs[b]*sizeof(float));
    memcpy(evi+_vc, eis[b], vcs[b]*sizeof(float));
    _vc += vcs[b];
  }

  // free memory
  for (int b = 0; b < nb; b++) {
    delete[] eis[b];
    delete[] ejs[b];
    delete[] evds[b];
    delete[] evis[b];
  }

  delete[] eis;
  delete[] ejs;
  delete[] evds;
  delete[] evis;

  delete[] bstarts;
  delete[] bstops;
  delete[] dstarts;
  delete[] dstops;
  delete[] vcs;

  return Image(ei, ej, evd, evi, vc, d);
}
#endif

Image im2gr(Data *data, const int d, ConstructionMode mode, diff_fn diff) {
  if (mode == SingleThread)
    return st_construct(data, d, diff);
#if MULTITHREAD
  if (mode == MultiThread)
    return mt_construct(data, d, diff);
#else
  if (mode == MultiThread) {
    std::cerr << "im2gr must be compiled with MULTITHREAD enabled." << std::endl;
    goto err;
  }
#endif
  std::cerr << "Unsupported construction mode." << std::endl;
err:
  std::cerr << "im2gr errored while trying to construct graph" << std::endl;
  exit(EXIT_FAILURE);
}

