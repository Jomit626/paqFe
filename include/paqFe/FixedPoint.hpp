#pragma once

#include <cstdint>

namespace paqFe::internal {

class SquashStretch {
  static uint32_t stretch_lut[4096];

private:

  void init_stretch() {
    uint32_t *lut = (uint32_t*)stretch_lut;

    int pi=0;
    for (int x=-2047; x<=2047; ++x) {  // invert squash()
      int i=squash(x);
      for (int j=pi; j<=i; ++j)
        lut[j]=x;
      pi=i+1;
    }
    lut[4095]=2047;
  }

public:
  SquashStretch() {
    init_stretch();
  }

  int squash(int d) {
    static const int t[33]={
      1,2,3,6,10,16,27,45,73,120,194,310,488,747,1101,
      1546,2047,2549,2994,3348,3607,3785,3901,3975,4022,
      4050,4068,4079,4085,4089,4092,4093,4094};
    if (d>2047) return 4095;
    if (d<-2047) return 0;
    int w=d&127;
    d=(d>>7)+16;
    return (t[d]*(128-w)+t[(d+1)]*w+64) >> 7;
  }

  int stretch(int x) {
    return stretch_lut[x];
  }
};

uint32_t SquashStretch::stretch_lut[4096] = {};
SquashStretch LUT;

}