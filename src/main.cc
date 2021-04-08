#include <iostream>

#include "cpu/diff-func.h"
#include "cpu/loader.h"

int main(int argc, char **argv) {
  if (argc < 3) {
    std::cerr << "usage: " << argv[0] << " <data path> <d>" << std::endl;
    exit(EXIT_FAILURE);
  }

  std::string path(argv[1]);
  int d = atoi(argv[1]);

  Loader<short> loader(path, d);
  loader.im2gr(mri_diff_func);
  return 0;
}
