#pragma once

#include "../types.hpp"

namespace paqFe {

template<int p = ProbEven>
class PresudoModel {
public:
  void init();

  Prob predict(uint8_t bit) {
    return ProbEven;
  };
  
  Context getContext() {
    return 0;
  };
};

}