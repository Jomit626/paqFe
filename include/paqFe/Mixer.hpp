#pragma once

#include <cstring>
#include <cassert>

#include "types.hpp"

namespace paqFe::internal {

template<int nFeature, int nHidden>
class Mixer {
protected:
  Weight W[nHidden][256][nFeature];
  int32_t X[nFeature];

  Weight W1[nHidden];
  int32_t X1[nHidden];

  Prob prev_prob = ProbEven;
  Context prev_ctx[nHidden];
public:

  Mixer() {
    memset(W, 0x00, sizeof(W));
    Weight w = (1 << 16) / nFeature;
    Weight w1 = (1 << 16) / nHidden;
    for(int k=0;k<nHidden;k++) {
      W1[k] = w;
      prev_ctx[k] = 0;
      X1[k] = 0;
      for(int j=0;j<256;j++)
        for(int i=0;i<nFeature;i++)
          W[k][j][i] = w;
    }

    for(int i=0;i<nFeature;i++)
      X[i] = LUT.stretch(ProbEven);
  }

  void predict(const Prob* P, const Context *pctx, Prob *pp) {
    for(int i=0;i<nFeature;i++)
      X[i] = LUT.stretch(P[i]);

    for(int k=0;k<nHidden;k++) {
      int ctx = pctx[k] & 0xFF;
      prev_ctx[k] = ctx;
      int32_t y = dot(X, W[k][ctx], nFeature);
      y = (y * 1) >> 16;
      X1[k] = LUT.stretch(LUT.squash(y));
    }
    *pp = prev_prob = LUT.squash( (dot(X1, W1, nHidden) * 1) >> 16 );
  }

  void update(uint8_t bit) {
    for(int k=0;k<nHidden;k++) {
      train(W[k][prev_ctx[k]], X, nFeature, LUT.squash(X1[k]), bit, 7);
    }
    train(W1, X1, nHidden, prev_prob, bit, 3);
  }

  void update(uint8_t bit, Prob p) {
    prev_prob = p;
    update(bit);
  }

private:
  int32_t dot(int32_t* a, int32_t* b, size_t n) {
    int32_t s = 0;
    for(int i=0;i<n;i++) {
      s += a[i] * b[i];
    }

    return s;
  }

  void train(Weight *w, int32_t *x, size_t len, Prob y, uint8_t bit, int lr) {
    int loss = ((bit << 12) - y) * lr;

    for(int i=0;i<len;i++) {
      Weight wt = w[i] + ((((x[i] * loss * 2) >> 16U) + 1) >> 1U);
      if( wt < -32768 ) {
        wt = -32768;
      } else if( wt > 32767 ) {
        wt = 32767;
      }
      w[i] = wt;
    }
  }

};

}