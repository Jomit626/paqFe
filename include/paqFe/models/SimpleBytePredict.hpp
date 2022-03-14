#pragma once

#include "../types.hpp"

namespace paqFe::internal {

template<typename Model>
class ModelSimpleBytePredict : protected Model {
protected:
  Prob pNext[Model::nProb];
  Context ctxNxt[Model::nCtx];

public:
  static constexpr int nProb = Model::nProb;
  static constexpr int nCtx = Model::nCtx;
  static constexpr int CtxShift = Model::CtxShift;

  ModelSimpleBytePredict() {
    for(int i=0;i<nProb;i++)
      pNext[i] = ProbEven;
    for(int i=0;i<nCtx;i++)
      ctxNxt[i] = 0;
  }

  void predict(uint8_t bit, Prob *pp, Context *pctx) {
    Model::predict(bit, pp, pctx);
  }

  void predict_byte(uint8_t byte, Prob *pp, Context *pctx, size_t pstride, size_t ctxstride) {
    for(int j=0;j<8;j++) {
      for(int i=0;i<nProb;i++)
        pp[j * pstride + i] = pNext[i];
      for(int i=0;i<nCtx;i++)
        pctx[j * ctxstride + i] = ctxNxt[i];
      
      uint8_t bit = BitSel(byte, j);
      predict(bit, &pNext[0], &ctxNxt[0]);
    }
  }

};

}