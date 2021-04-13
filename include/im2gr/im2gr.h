#ifndef _IM2GR_H_
#define _IM2GR_H_

#include "im2gr/data.h"
#include "im2gr/image.h"

enum ConstructionMode {
  SingleThread,
  MultiThread
};

using diff_fn = float (*)(float, float);

Image im2gr(Data *data, const int d, ConstructionMode mode, diff_fn diff);

Image st_construct(Data *data, const int d, diff_fn diff);

#if MULTITRHEAD
Image mt_construct(Data *data, const int d, diff_fn diff);
#endif

#endif /* _IM2GR_H_ */
