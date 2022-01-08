#pragma once

#include <cstddef>
#include <cstdint>

#include "FixedPoint.hpp"

namespace paqFe {

enum OpMode {
  Compress,
  Decompress
};

namespace internal {

typedef int32_t Weight;
typedef uint32_t Prob;
typedef uint8_t Context;

constexpr Prob ProbEven = 2048;
constexpr Prob ProbFirst = 2048;

template<typename T>
constexpr bool isPow2(const T& t) {
  return (t > 0 && (((t & (t - 1)) == 0)));
}

static constexpr int max(int a, int b) {
  if(a > b)
    return a;
  else
    return b;
}

}
}