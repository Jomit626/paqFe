#pragma once

#include "../types.hpp"

namespace paqFe::internal {

template<int p = ProbEven>
class PresudoModel {
public:
  static constexpr int OutputCnt = 1;

  void init();

  void predict(uint8_t bit, Prob* pp) {
    *pp = p - 1 + bit * 2;
  };

};

}