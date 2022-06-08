#pragma once

#include <cstdint>

namespace paqFe {

struct Header
{
  union {
    uint8_t b[8];
    uint64_t dw;
  } origin_size;
  
  union {
    uint8_t b[4];
    uint32_t dw;
  } crc32_check_sum;
};

}
