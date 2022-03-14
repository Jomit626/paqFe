#pragma once

#include "../types.hpp"

namespace paqFe::internal {

template<typename ... Models>
class ModelGroup : Models... {
private:

#define AttributeSumDeclare(Attribute)  \
  template<typename M, typename ... Rest> \
  static constexpr int Attribute##Sum() { \
    if constexpr (sizeof...(Rest)) {  \
      return Attribute##Sum<Rest...>() + M::Attribute; \
    } else {  \
      return M::Attribute;  \
    } \
  }

  AttributeSumDeclare(nProb)
  AttributeSumDeclare(nCtx)
  AttributeSumDeclare(CtxShift)

#undef AttributeSumDeclare
public:

  static constexpr int nProb = nProbSum<Models...>();
  static constexpr int nCtx = nCtxSum<Models...>();
  static constexpr int CtxShift = CtxShiftSum<Models...>();

  void predict(uint8_t bit, Prob *pp, Context* pctx) {
    predict<Models...>(bit, pp, pctx);
  }

  void predict_byte(uint8_t byte, Prob *pp, Context* pctx) {
    predict_byte<Models...>(byte, pp, pctx);
  }

  Context ContextMix(const Context *pctx) {
    return ContextMix<Models...>(pctx);
  }

private:

  template<typename M, typename ... Rest>
  void predict(uint8_t bit, Prob *pp, Context* pctx) {
    M::predict(bit, pp, pctx);

    if constexpr (sizeof...(Rest)) {
      predict<Rest...>(bit, pp + M::nProb, pctx + M::nCtx);
    }
  }

  template<typename M, typename ... Rest>
  void predict_byte(uint8_t byte, Prob *pp, Context* pctx) {
    M::predict_byte(byte, pp, pctx, nProb, nCtx);

    if constexpr (sizeof...(Rest)) {
      predict_byte<Rest...>(byte, pp + M::nProb, pctx + M::nCtx);
    }
  }

  template<typename M, typename ... Rest>
  Context ContextMix(const Context *pctx) {
    Context c = 0;
    for(int i=0;i<nCtx;i++) {
      c = (c << 1) + pctx[i];
    }
    return c;
    /*
    if constexpr (sizeof...(Rest)) {
      if constexpr (M::nCtx) {
        return *pctx ^ (ContextMix<Rest...>(pctx + 1) << M::CtxShift);
      } else {
        return ContextMix<Rest...>(pctx);
      }
    } else {
      if constexpr (M::nCtx) {
        return *pctx;
      } else {
        return 0;
      }
    }
    */
  }

};

}
