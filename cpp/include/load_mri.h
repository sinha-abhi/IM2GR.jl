#ifndef _LOAD_MRI_H_
#define _LOAD_MRI_H_

#include <algorithm>
#include <cmath>

typedef float (*diff_func_call)(float x, float y);

float mri_diff_func(float x, float y) {
    return (std::min(sqrt(x) / 63.0, 1.0) - std::min(sqrt(y) / 63.0, 1.0));
}

/**
 *	im    : image
 *	im_sz : image size (x, y, z)
 *	d 	  : distance of neighbors
 *	diff  : differential function
 */
template<typename T>
void im2gr(Index *ei, Index *ej, float *evd, float *evi,
           T ***im, int *im_sz, int d, diff_func_call diff) {
    short max_x = im_sz[0] - 1;
    short max_y = im_sz[1] - 1;
    short max_z = im_sz[2] - 1;

    short x, y, z;
    short nx, ny, nz;

    short low_x, low_y, low_z;
    short up_x, up_y, up_z;
    Index src, dest;
    Index idx_low, idx_up;

    T pi, pj;
    float dist, dst_sq;

    unsigned vc = 0;
    for (x = 0; x <= max_x; ++x) {
#if DEBUG
        std::cout << "x: " << x << std::endl;
#endif
        for (y = 0; y <= max_y; ++y) {
#if DEBUG
            std::cout << "y: " << y << std::endl;
#endif
            for (z = 0; z <= max_z; ++z) {
#if DEBUG
                std::cout << "x: " << x << " y: " << y << " z: " << z << " vc: " << vc << std::endl;
#endif
                low_x = x - d;
                low_y = y - d;
                low_z = z - d;

                if (low_x < 0)
                    idx_low.set_x(0);
                else
                    idx_low.set_x(low_x);

                if (low_y < 0)
                    idx_low.set_y(0);
                else
                    idx_low.set_y(low_y);

                if (low_z < 0)
                    idx_low.set_z(0);
                else
                    idx_low.set_z(low_z);

                up_x = x + d;
                up_y = y + d;
                up_z = z + d;

                if (up_x > max_x)
                    idx_up.set_x(max_x);
                else
                    idx_up.set_x(up_x);

                if (up_y > max_y)
                    idx_up.set_y(max_y);
                else
                    idx_up.set_y(up_y);

                if (up_z > max_z)
                    idx_up.set_z(max_z);
                else
                    idx_up.set_z(up_z);

                src.set_x(x);
                src.set_y(y);
                src.set_z(z);
                pi = im[x][y][z];
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
                            pj = im[nx][ny][nz];

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
#if DEBUG
    std::cout << "vc: " << vc << std::endl;
#endif
}

#endif /* _LOAD_MRI_H_ */
