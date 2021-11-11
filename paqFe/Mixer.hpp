#pragma once

#include "types.hpp"

namespace paqFe {

template<typename ... Models>
class Mixer : Models... {

  static constexpr int n = sizeof...(Models);

  Weight W[n][256];
  Prob X[n];
  Context Ctx[n];
  
  template<typename M, typename ... Rest>
  void predict(uint8_t bit, Prob *pp, Context* ctx) {
    *pp = M::predict(bit);
    *ctx = M::getContext();

    if constexpr (sizeof...(Rest)) {
      predict<Rest...>(bit, ++pp, ++ctx);
    }
  }

  uint32_t dot(Weight* w, Prob* p) {
    uint32_t s = 0;
    for(int i=0;i<n;i++) {
      s += w[i] * p[i];
    }

    return s;
  }

public:

  Mixer() {}

  Mixer(Models&&... models) 
    : Models(models)...{

  }

  void init() {
    
  }

  Prob predict(uint8_t bit) {
    predict<Models...>(bit, X, Ctx);
    int sum = 0;
    for(int i=0;i<n;i++)
      sum += X[i];
    return sum;
  }

  Prob predict_batch(uint8_t bit) {
    // TODO: batch mode
    return 0;
  }
};

}