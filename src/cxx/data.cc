#include <iostream>

#include "im2gr/data.h"
#include "im2gr/utils.h"

Data::Data(std::string f) {
  sz = new size_t[3];

  data = new uint8_t**[MAX_X];
  for (int i = 0; i < MAX_X; ++i) {
    data[i] = new uint8_t*[MAX_Y];
    for (int j = 0; j < MAX_Y; ++j)
      data[i][j] = new uint8_t[MAX_Z];
  }

  auto _data(parse_file(sz, f));
#ifdef DEBUG
  std::cout << "Parsed file." << std::endl;
#endif

  unflatten(data, sz, _data);
#ifdef DEBUG
  std::cout << "Expanded Image." << std::endl;
#endif

#ifdef DEBUG
  std::cout << "Image size: " << sz[0] << ", "
            << sz[1] << ", "
            << sz[2] << std::endl;
#endif
}

Data::Data(uint8_t ***data, size_t *sz) : data(data), sz(sz) {
#if DEBUG
  std::cout << "Image size: " << sz[0] << ", "
            << sz[1] << ", "
            << sz[2] << std::endl;
#endif
  // empty
}

Data::~Data() {
  delete[] sz;
  for (int i = 0; i < MAX_X; ++i) {
    for (int j= 0; j < MAX_Y; ++j)
      delete[] data[i][j];
    delete[] data[i];
  }
  delete[] data;
}

uint8_t ***Data::get_data() {
  return data;
}

size_t *Data::get_size() {
  return sz;
}

