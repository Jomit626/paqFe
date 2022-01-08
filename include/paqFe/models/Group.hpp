#pragma once

#include "../types.hpp"

namespace paqFe::internal {

template<typename ... Models>
class ModelGroup : Models... {
private:

  template<typename M, typename ... Rest>
  static constexpr int getOutputCnt() {
    if constexpr (sizeof...(Rest)) {
      return getOutputCnt<Rest...>() + M::OutputCnt;
    } else {
      return M::OutputCnt;
    }
  }

public:
  static constexpr int OutputCnt = getOutputCnt<Models...>();

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
      predict<Rest...>(bit, pp + M::OutputCnt, pctx + M::OutputCnt);
    }
  }

  template<typename M, typename ... Rest>
  void predict_byte(uint8_t byte, Prob *pp, Context* pctx) {
    M::predict_byte(byte, pp, pctx, OutputCnt);

    if constexpr (sizeof...(Rest)) {
      predict_byte<Rest...>(byte, pp + M::OutputCnt, pctx + M::OutputCnt);
    }
  }

};

}
