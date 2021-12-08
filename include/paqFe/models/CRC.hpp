#pragma once

#include "../types.hpp"

namespace paqFe::internal {

template<size_t Width>
class CRC {

  static constexpr uint32_t sel_poly(size_t width) {
    if(width == 32)
      return 0x04C11DB7L;
    else if(width == 16)
      return 0x1021;
    else
      return 0;
  }

  static constexpr uint32_t poly = sel_poly(Width);
  static_assert(poly != 0, "CRC width error, no poly defined for width");

  uint32_t table[256];
public:
  CRC() {
    gen_table();
  };

  uint32_t next(uint32_t crc, uint8_t byte) {
    crc = (crc << 8 ) ^ table[(crc >> 24) ^ byte];
    return crc;
  }

private:
  
  void gen_table() {
    for(int i=0;i<256;i++) {
      table[i] = gen_table_entry(i);
    }
  }

  uint32_t gen_table_entry(int idx) {
    constexpr uint32_t top = (uint32_t)1 << (Width - 1);
    uint32_t reg = (uint32_t)idx << (Width - 8);

    for(int i=0;i<8;i++) {
      if(reg & top)
        reg = (reg << 1) ^ poly;
      else
        reg = (reg << 1);
    }

    return reg & (((uint64_t)1 << Width) - 1);
  }
};


}