#ifndef _DIFF_FUNC_H_
#define _DIFF_FUNC_H_

#include <cmath>

inline float mri_diff_func(float x, float y) {
  return (float) (std::min(sqrt(x)/63.0, 1.0) - std::min(sqrt(y)/63.0, 1.0));
}

#endif /* _DIFF_FUNC_H_ */
