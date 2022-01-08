#pragma once

#include <cstring>
#include <cassert>

#include "types.hpp"

namespace paqFe::internal {

template<int nfeature>
class Mixer {
  Weight W[256][nfeature];
  int32_t X[nfeature] = { ProbEven };

  Prob prev_prob = ProbEven;
  Context prev_ctx = 0;
public:

  Mixer() {
    memset(W, 0x00, sizeof(W));

    for(int i=0;i<nfeature;i++)
      X[i] = LUT.stretch(ProbEven);
  }

  void predict(const Prob* P, const Context ctx, Prob *pp) {
    for(int i=0;i<nfeature;i++)
      X[i] = LUT.stretch(P[i]);

    *pp = prev_prob = LUT.squash( dot(X, W[ctx], nfeature) >> 16 );
    prev_ctx = ctx;
  }

  void update(uint8_t bit) {
    train(W[prev_ctx], X, prev_prob, bit);
  }

private:
  int32_t dot(int32_t* a, int32_t* b, int n) {
    int32_t s = 0;
    for(int i=0;i<n;i++) {
      s += a[i] * b[i];
    }

    return s;
  }

  void train(Weight *w, int32_t *x, Prob y, uint8_t bit) {
    int loss = ((bit << 12) - y) * 30;

    for(int i=0;i<nfeature;i++)
      w[i] = w[i] + ((x[i] * loss) >> 16);
  }

};

}