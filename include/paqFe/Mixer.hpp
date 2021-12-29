#pragma once

#include <cstring>
#include <cassert>

#include "types.hpp"

namespace paqFe::internal {

static int squash(int d) {
  static const int t[33]={
    1,2,3,6,10,16,27,45,73,120,194,310,488,747,1101,
    1546,2047,2549,2994,3348,3607,3785,3901,3975,4022,
    4050,4068,4079,4085,4089,4092,4093,4094};
  if (d>2047) return 4095;
  if (d<-2047) return 0;
  int w=d&127;
  d=(d>>7)+16;
  return (t[d]*(128-w)+t[(d+1)]*w+64) >> 7;
}

template<int N, typename ... Models>
class Mixer : Models... {

  static constexpr int nmodels = sizeof...(Models);

  template<typename M, typename ... Rest>
  static constexpr int getFeaturesCnt() {
    if constexpr (sizeof...(Rest)) {
      return getFeaturesCnt<Rest...>() + M::OutputCnt;
    } else {
      return M::OutputCnt;
    }
  }
  static constexpr int nfeatures = getFeaturesCnt<Models...>();

  bool first = true;
  uint8_t ctx = 0;
  uint8_t ctx_mask = 1;
  int cnt = 0;

  int duty = 0;

  Weight W[N][256][nfeatures];  // 256 for per feature context

  int32_t X[nfeatures] = { ProbEven };
  Prob P[nfeatures];

  Context Ctx[N];

  Prob prev_prob = ProbEven; // stretch(ProbEven)

  uint32_t stretch[4096];
  void init_stretch() {
    int pi=0;
    for (int x=-2047; x<=2047; ++x) {  // invert squash()
      int i=squash(x);
      for (int j=pi; j<=i; ++j)
        stretch[j]=x;
      pi=i+1;
    }
    stretch[4095]=2047;
  }
public:
  static constexpr int OutputCnt = 1;

  Mixer() {
    memset(W, 0x00, sizeof(W));
    memset(Ctx, 0x00, sizeof(Ctx));
    
    init_stretch();

    for(int i=0;i<nfeatures;i++)
      X[i] = stretch[ProbEven];
  }

  Mixer(Models&&... models) 
    : Models(models)...{
  }

  void predict(uint8_t bit, Prob *pp) {
    train(W[duty][Ctx[duty]], X, prev_prob, bit);
    
    duty = (duty + 1) % N;
    Context c0[nfeatures];

    predict<Models...>(bit, P, c0);
    Ctx[duty] = contextmix(c0, nfeatures);

    for(int i=0;i<nfeatures;i++) {
      X[i] = stretch[P[i]];
    }

    *pp = prev_prob = squash( dot(X, W[duty][Ctx[duty]], nfeatures) >> 16 );
    return;
  }

  Context contextmix(Context *pctx, size_t n) {
    Context c = 0;
    for(int i=0;i<n;i++) {
      c += pctx[i];
    }
    return c;
  }

  void predict_byte(uint8_t byte, Prob *pp) {
    assert(("", duty == 0));
    static_assert(("Only support N=8", N == 8));

    Prob x0[8 * nfeatures];
    Context c0[8 * nfeatures];
    int32_t x[8 * nfeatures];
    predict_byte<Models...>(byte, &x0[0], &c0[0]);
    for(int i=0;i<nfeatures * 8;i++) {
      x[i] = stretch[x0[i]];
    }
    for(int duty=0;duty<N;duty++) {
      Ctx[duty] = contextmix(&c0[duty * nfeatures], nfeatures);
    }

    for(int duty=0;duty<N;duty++) {
      pp[duty] = squash( dot(&x[nfeatures * duty], W[duty][Ctx[duty]], nfeatures) >> 16 );
    }

    if(first) {
      pp[0] = ProbEven;
      first = false;
    }

    for(int duty=0;duty<N;duty++) {
      train(W[duty][Ctx[duty]], &x[nfeatures * duty],  pp[duty], (byte >> (7 - duty)) & 0x1);
    }
    return ;
  }

private:

  template<typename M, typename ... Rest>
  void predict(uint8_t bit, Prob *pp, Context* pctx) {
    M::predict(bit, pp, pctx);

    if constexpr (sizeof...(Rest)) {
      predict<Rest...>(bit, pp + M::OutputCnt, pctx + M::OutputCnt);
    }
  }

  template<typename M, typename ... Rest>
  void predict_byte(uint8_t byte, Prob *pp, Context* pctx) {
    M::predict_byte(byte, pp, pctx, nfeatures);

    if constexpr (sizeof...(Rest)) {
      predict_byte<Rest...>(byte, pp + M::OutputCnt, pctx + M::OutputCnt);
    }
  }

  int32_t dot(int32_t* a, int32_t* b, int n) {
    int32_t s = 0;
    for(int i=0;i<n;i++) {
      s += a[i] * b[i];
    }

    return s / n;
  }

  void train(uint8_t bit) {
    train(W[duty], X, prev_prob, bit);
  }

  void train(Weight *w, int32_t *x, Prob y, uint8_t bit) {
    int loss = ((bit << 12) - y) * 80;

    for(int i=0;i<nfeatures;i++)
      w[i] = w[i] + ((x[i] * loss) >> 16);
  }
};

}