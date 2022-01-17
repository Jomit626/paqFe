#pragma once

#include "../types.hpp"

namespace paqFe::internal {

template<typename ... Models>
class ModelGroup : Models... {
private:

  template<typename M, typename ... Rest>
  static constexpr int getNProb() {
    if constexpr (sizeof...(Rest)) {
      return getNProb<Rest...>() + M::nProb;
    } else {
      return M::nProb;
    }
  }

  template<typename M, typename ... Rest>
  static constexpr int getNCtx() {
    if constexpr (sizeof...(Rest)) {
      return getNCtx<Rest...>() + M::nCtx;
    } else {
      return M::nCtx;
    }
  }

public:
  static constexpr int nProb = getNProb<Models...>();
  static constexpr int nCtx = getNCtx<Models...>();

  void predict(uint8_t bit, Prob *pp, Context* pctx) {
    predict<Models...>(bit, pp, pctx);
  }

  void predict_byte(uint8_t byte, Prob *pp, Context* pctx) {
    predict_byte<Models...>(byte, pp, pctx);
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

};

}
