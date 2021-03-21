#include <iostream>

#include "cpu/diff-func.h"
#include "cpu/loader.h"

int main(int argc, char **argv) {
  int d = 1;
  if (argc < 2)
    std::cerr << "Defaulting to d = " << d << std::endl;
  else
    d = atoi(argv[1]);

  Loader<short> loader("data/lgemri.txt", d);
  loader.im2gr(mri_diff_func);
  return 0;
}
