#pragma once

#include "../types.hpp"

namespace paqFe::internal {

template<Prob ... probs>
class ConstantModel {
  static constexpr int nProb = sizeof...(probs);
  static constexpr int nCtx = 0;
};

class BiasModel {
public:
  static constexpr Prob p = 256;
  static constexpr int nProb = 1;
  static constexpr int nCtx = 0;

  bool first = true;

  void predict(uint8_t bit, Prob *pp, Context *pctx) {
    *pp = p;
  }
  void predict_byte(uint8_t byte, Prob *pp, Context *pctx, size_t pstride, size_t ctxstride) {
    for(int i=0;i<8;i++)
      pp[i * pstride] = p;
    if(first) {
      pp[0] = ProbEven;
      first = false;
    }
  }
};

}