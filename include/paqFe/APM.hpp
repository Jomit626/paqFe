#pragma once

#include "types.hpp"
#include <cstdio>

namespace paqFe::internal {

template<size_t Size>
class APM {
  struct entry
  {
    int32_t prob; // fixed point 16
    int cnt;
  } entries[Size];

  size_t context = 0;

public:
  APM() {
    for (int i=0; i<Size; ++i) {
      int p=((i%24*2+1)*4096)/48-2048;
      entries[i].prob = LUT.squash(p) << 4;
      entries[i].cnt = 6;
    }
  };

  void predict(Context ctx, Prob pin, Prob *pp) {
    Prob p = (LUT.stretch(pin)+2048)*23;
    int weight = p & 0xFFF;
    context = ctx * 24 + (p >> 12);

    assert(context < Size);
    *pp = entries[context].prob * (0x1000 - weight) + entries[context + 1].prob * weight >> 16;

    context = context + (weight >> 11);  // +(0 or 1)
  };

  void update(uint8_t bit) {
    //printf("update:%d\n",bit);
    entry &e = entries[context];
    e.prob += (((int32_t)bit << 16) - e.prob) * 2 / (2 * e.cnt + 3);
    e.cnt = max(1024, e.cnt + 1);
  }
};


}