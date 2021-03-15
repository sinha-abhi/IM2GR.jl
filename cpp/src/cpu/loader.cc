#include <iostream>

#include "loader.h"

// TODO: make ops atomic for mt
static float mri_diff_func(float x, float y) {
    return (std::min(sqrt(x) / 63.0, 1.0) - std::min(sqrt(y) / 63.0, 1.0));
}

Loader::Loader(const char *f, int dist) : d(dist) {
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

    unsigned int upv = graph_vector_ub(sz, d);

    ei = new Index[upv];
    ej = new Index[upv];
    evi = new float[upv];
    evd = new float[upv];

#ifdef DEBUG
    std::cout << "Image size: " << sz[0] << ", "
                                << sz[1] << ", "
                                << sz[2] << std::endl;
    std::cout << "Result vector upperbound: " << upv << std::endl;
#endif
}

Loader::~Loader() {
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

void Loader::_find_nghbrs(Index begin, Index end) {
    int x = begin.x();
    int y = begin.y();
    int z = begin.z();

    int max_x = end.x();
    int max_y = end.y();
    int max_z = end.z();

    Index src, dest;
    Index idx_low, idx_up;

    std::pair<Index, Index> bounds;

    int nx, ny, nz;
    uint8_t pi, pj;
    float dist, dst_sq;
    for (x = 0; x <= max_x; ++x) {
        for (y = 0; y <= max_y; ++y) {
            for (z = 0; z <= max_z; ++z) {
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
                            evi[vc] = mri_diff_func(pi, pj);

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

void Loader::im2gr() {
    vc = 0;

#ifdef MULTITHREAD
#ifdef DEBUG
    std::cout << "Processing image... MULTITHREAD" << std::endl;
#endif
    // TODO: divide into blocks
#else
#ifdef DEBUG
    std::cout << "Processing image... SINGLETHREAD" << std::endl;
#endif
    Index begin;
    Index end(sz[0]-1, sz[1]-1, sz[2]-1);

    _find_nghbrs(begin, end);
#endif
}
