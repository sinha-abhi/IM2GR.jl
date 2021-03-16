#ifndef _MRILOADER_H_
#define _MRILOADER_H_

#include <algorithm>
#include <cctype>
#include <cmath>

#ifdef MULTITHREAD
#include <boost/thread.hpp>
#include <mutex>
#endif

#include "index.h"
#include "utils.h"

#define MAX_X 600
#define MAX_Y 600
#define MAX_Z 100

using diff_fn = float (*)(float, float);

template <typename T>
class Loader {
public:
  Loader(const char *fname, int d);
  ~Loader();
  int*      get_sz()  { return sz;  }
  unsigned  get_vc()  { return vc;  }
  Index<T>* get_ei()  { return ei;  }
  Index<T>* get_ej()  { return ej;  }
  float*    get_evi() { return evi; }
  float*    get_evd() { return evd; }

  void im2gr(diff_fn diff);

private:
  uint8_t   ***data;
  int         *sz;
  const int    d;
  unsigned     vc;
  Index<T>    *ei, *ej;
  float       *evi, *evd;

#ifdef MULTITHREAD
  boost::shared_mutex d_mut;
  std::mutex          v_mut;
#endif

  void _find_nghbrs(diff_fn diff, Index<T> begin, Index<T> end);
};


#endif /* _MRILOADER_H_ */
