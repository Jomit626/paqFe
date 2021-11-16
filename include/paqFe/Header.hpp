#pragma once

#include <cstdint>
namespace paqFe {

struct Header
{
  union {
    uint8_t b[8];
    uint64_t dw;
  } origin_size;
};

}
