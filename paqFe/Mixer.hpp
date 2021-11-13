#pragma once

#include "types.hpp"

namespace paqFe {

template<typename ... Models>
class Mixer : Models... {

  static constexpr int n = sizeof...(Models);

  //Weight W1[n][256][n];
  Weight W[n];
  Prob X[n];
  //Prob M[n];
  Context Ctx[n];

  Prob prev_prob = ProbEven;
public:

  Mixer() {}

  Mixer(Models&&... models) 
    : Models(models)...{
    // getContext();
  }

  void init() {
    
  }

  Prob predict(uint8_t bit) {
    train(bit);

    predict<Models...>(bit, X, Ctx);
    Prob p = dot(X, W);

    return p;
  }

  Prob predict_batch(uint8_t bit) {
    // TODO: batch mode
    return 0;
  }

private:

  template<typename M, typename ... Rest>
  void predict(uint8_t bit, Prob *pp, Context* ctx) {
    *pp = M::predict(bit);
    *ctx = M::getContext();

    if constexpr (sizeof...(Rest)) {
      predict<Rest...>(bit, ++pp, ++ctx);
    }
  }

  template<typename M, typename ... Rest>
  void getContext(Context* ctx) {
    *ctx = M::getContext();

    if constexpr (sizeof...(Rest)) {
      getContext<Rest...>(++ctx);
    }
  }

  uint32_t dot(Weight* w, Prob* p) {
    uint32_t s = 0;
    for(int i=0;i<n;i++) {
      s += w[i] * p[i];
    }

    return s;
  }

  void train(uint8_t bit) {
    uint32_t loss = (bit << 12) - prev_prob;
    uint32_t lr = 1;
    
    for(int i=0;i<n;i++)
      W[i] = W[i] + X[i]*loss*lr;
  }

};

}