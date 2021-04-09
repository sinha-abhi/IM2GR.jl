#ifndef _IMAGE_H_
#define _IMAGE_H

#include "index.h"

class Image {
public:
  Image(Index *ei, Index *ej, float *evd, float *evi, size_t vc, int d);
  ~Image();
  Index  *get_ei();
  Index  *get_ej();
  float  *get_evd();
  float  *get_evi();
  size_t  get_vc();
  int     get_d();

private:
  Index     *ei, *ej;
  float     *evd, *evi;
  size_t     vc;
  const int  d;
};

#endif /* _IMAGE_H_ */
