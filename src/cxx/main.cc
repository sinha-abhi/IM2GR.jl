#include <iostream>

#include "im2gr/diff-func.h"
#include "im2gr/im2gr.h"

int main(int argc, char **argv) {
  if (argc < 3) {
    std::cerr << "usage: " << argv[0] << " <data path> <d>" << std::endl;
    exit(EXIT_FAILURE);
  }

  std::string path(argv[1]);
  int d = atoi(argv[2]);

  Data *data = new Data(path);
#if MULTITHREAD
  auto image = im2gr(data, d, MultiThread, mri_diff_func);
#else
  auto image = im2gr(data, d, SingleThread, mri_diff_func);
#endif

  delete data;

  return 0;
}
