#ifndef _DIFF_FN_H_
#define _DIFF_FN_H_

#include <cmath>

inline float mri_diff_func(float x, float y) {
  return (std::min(sqrt(x) / 63.0, 1.0) - std::min(sqrt(y) / 63.0, 1.0));
}

#endif /* _DIFF_FN_H_ */
