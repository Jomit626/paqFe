#pragma once

#include <cstddef>
#include <cstdint>

namespace paqFe {

typedef uint16_t Weight;
typedef uint16_t Prob;
typedef uint8_t Context;

constexpr Prob ProbEven = 2048;

enum OpMode {
  Comresss,
  Decompress
};

template<typename T>
constexpr bool isPow2(const T& t) {
  return (t > 0 && (((t & (t - 1)) == 0)));
}

}