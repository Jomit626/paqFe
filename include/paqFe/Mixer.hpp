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
  Weight dW1[nHidden];
  int32_t X1[nHidden];

  Prob prev_prob = ProbEven;
  Context prev_ctx[nHidden];
  int counter = 0;
  static constexpr int BatchSize = 1024;
  static constexpr int Layer1LR = 7;
  static constexpr int Layer2LR = 3;
public:

  Mixer() {
    memset(dW1, 0x00, sizeof(dW1));
    Weight w = (1 << 16) / nFeature;
    Weight w1 = (1 << 16) / nHidden;
    for(int k=0;k<nHidden;k++) {
      W1[k] = w1;
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
      int32_t y = dot(X, W[k][ctx], nFeature) >> 16;
      if(y > 2047)
        y = 2047;
      else if(y <= -2048)
        y = -2048;
      X1[k] = y;
    }
    int y = (dot(X1, W1, nHidden)) >> 16;
    if(y > 2047)
      y = 2047;
    else if(y <= -2048)
      y = -2048;
    *pp = prev_prob = LUT.squash(y);
  }

  void update(uint8_t bit) {
    for(int k=0;k<nHidden;k++) {
      train(W[k][prev_ctx[k]], X, nFeature, LUT.squash(X1[k]), bit, Layer1LR);
    }
    accLoss(X1, nHidden, prev_prob, bit, Layer2LR, dW1);
    counter ++;
    if(counter  == BatchSize) {
      counter = 0;
      vecAdd(W1, dW1, nHidden);
      memset(dW1,0,sizeof(dW1));
    }
  }

  void update(uint8_t bit, Prob p) {
    prev_prob = p;
    update(bit);
  }

protected:
  int32_t dot(int32_t* a, int32_t* b, size_t n) {
    int32_t s = 0;
    for(int i=0;i<n;i++) {
      s += a[i] * b[i];
    }

    return s;
  }

  void accLoss(int32_t *x, size_t len, Prob y, uint8_t bit, int lr, Weight *dws) {
    int loss = lossCal(y, bit, lr);

    for(int i=0;i<len;i++) {
      Weight dw = dws[i] + ((x[i] * loss ) >> 16U);
      dws[i] = dw;
    }
  }

  void vecAdd(int32_t* a, int32_t* b, size_t n){
    for(int i=0;i<n;i++) {
      a[i] += b[i];
    }
  }

  int lossCal(Prob y, uint8_t bit, int lr) {
    return ((bit << 12) - y) * lr;
  }

  void train(Weight *w, int32_t *x, size_t len, Prob y, uint8_t bit, int lr) {
    int loss = lossCal(y, bit, lr);

    for(int i=0;i<len;i++) {
      w[i] = w[i] + ((x[i] * loss ) >> 16U);
    }
  }

};

}