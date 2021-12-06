#pragma once

#include "types.hpp"

namespace paqFe {

class Coder {
protected:
  uint32_t low, high;

public:
  Coder() {
    low = 0x0;
    high = 0xFFFFFFFF;
  };

  void init()  {
    low = 0x0;
    high = 0xFFFFFFFF;
  };

  size_t encode(uint8_t bit, const Prob prob, uint8_t *output) {
    uint32_t range_d = high - low;
    uint64_t mul = (uint64_t)range_d * prob;
    uint32_t mid = low + (mul >> 12);

    if(bit)
      high = mid;
    else
      low = mid + 1;
    
    size_t n = 0;
    if (((high ^ low) >> 24) == 0) {
      output[n] = (high >> 24) & 0xFF;
      n = 1;

      high = (high << 8) | 0xFF;
      low = (low << 8);
    }
    return n;
  };

  size_t decode(uint32_t pos, const Prob prob, uint8_t *byte) {
    uint32_t range_d = high - low;
    uint64_t mul = (uint64_t)range_d * prob;
    uint32_t mid = low + (mul >> 12);

    if(pos <= mid) {
      *byte = (*byte << 1) | 0x01;
      high = mid;
    } else { 
      *byte = *byte << 1;
      low = mid + 1;
    }

    size_t n = 0;
    if (((high ^ low) >> 24) == 0) {
      n = 1;

      high = (high << 8) | 0xFF;
      low = (low << 8);
    }
    return n;
  };

  uint32_t get_tail() {
    return low;
  }

};

}
