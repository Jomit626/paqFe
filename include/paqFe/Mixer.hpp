#pragma once

#include <cstring>
#include "types.hpp"

namespace paqFe {

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
      return getFeaturesCnt<Rest...>() + M::n_output;
    } else {
      return M::n_output;
    }
  }
  static constexpr int nfeatures = getFeaturesCnt<Models...>();

  uint8_t ctx = 0;
  uint8_t ctx_mask = 1;
  int cnt = 0;

  //Weight W1[n][256][n];
  int duty = 0;
  Weight W[N][nfeatures];
  int32_t X[nfeatures];
  Prob P[nfeatures];

  Context Ctx[nmodels];

  Prob prev_prob = ProbEven;

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
  static constexpr int n_output = 1;

  Mixer() {
    memset(W, 0x00, sizeof(W));
    init_stretch();
  }

  Mixer(Models&&... models) 
    : Models(models)...{
    // getContext();
  }

  void init() {
    
  }

  void predict(uint8_t bit, Prob *pp) {
    train(bit);

    predict<Models...>(bit, P, Ctx);
    for(int i=0;i<nfeatures;i++)
      X[i] = stretch[P[i]];

    duty = (duty + 1) % N;
    *pp = prev_prob = squash( dot(X, W[duty], nfeatures) >> 16 );
    return;
  }

  Prob predict_batch(uint8_t bit) {
    // TODO: batch mode
    return 0;
  }

private:

  template<typename M, typename ... Rest>
  void predict(uint8_t bit, Prob *pp, Context* ctx) {
    M::predict(bit, pp);
    *ctx = M::getContext();

    if constexpr (sizeof...(Rest)) {
      predict<Rest...>(bit, pp + M::n_output , ++ctx);
    }
  }

  template<typename M, typename ... Rest>
  void getContext(Context* ctx) {
    *ctx = M::getContext();

    if constexpr (sizeof...(Rest)) {
      getContext<Rest...>(++ctx);
    }
  }

  int32_t dot(int32_t* a, int32_t* b, int n) {
    int32_t s = 0;
    for(int i=0;i<n;i++) {
      s += a[i] * b[i];
    }

    return s;
  }

  void train(uint8_t bit) {
    int loss = ((bit << 12) - prev_prob) * 16;

    for(int i=0;i<nfeatures;i++)
      W[duty][i] = W[duty][i] + ((X[i] * loss) >> 16);
  }

};

}