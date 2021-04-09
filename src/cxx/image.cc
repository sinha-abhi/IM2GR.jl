#include <iostream>

#include "im2gr/image.h"

Image::Image(Index *ei, Index *ej, float *evd, float *evi, size_t vc, int d) 
  : ei(ei), ej(ej),
    evd(evd), evi(evi),
    vc(vc), d(d) {
  // empty
}

Image::~Image() {
  delete[] ei;
  delete[] ej;
  delete[] evd;
  delete[] evi;
}

Index *Image::get_ei() {
  return ei;
}

Index *Image::get_ej() {
  return ej;
}

float *Image::get_evd() {
  return evd;
}

float *Image::get_evi() {
  return evi;
}

size_t Image::get_vc() {
  return vc;
}

int Image::get_d() {
  return d;
}
