#pragma once

#include <cstring>
#include <cmath>

#include "../types.hpp"
#include "StateMap.hpp"
#include "ContextMap.hpp"

#include "TabHash.hpp"
#include "TabHash.h"

namespace paqFe::internal {

template< size_t O2Size = 1ul << 20,
          size_t O3Size = 1ul << 20,
          size_t O4Size = 1ul << 21>
class NewOrders {
protected:
  // bit indicators in bitwise prediction
  int counter = 0;

  // bytewise context
  uint8_t C0 = 0; // current byte
  uint32_t C1 = 0;     // prev byte
  uint32_t C2 = 0;
  uint32_t C3 = 0;
  uint64_t C4 = 0;
  uint64_t C = 0;

  static size_t O1Hash(uint64_t val) {
    return val & ((1 << 12) - 1);
  }
  static size_t O2Hash(uint64_t val) {
    return tab_hashing<21, 16>(O2HashTab, val);
  }
  static size_t O3Hash(uint64_t val) {
    return tab_hashing<29, 16>(O3HashTab, val);
  }
  static size_t O4Hash(uint64_t val) {
    return tab_hashing<34, 17>(O4HashTab, val);
  }
  static size_t MulHash(uint64_t val) {
    return (val * 	10000000807) ^ ((val >> 32) * 123456);
  }

  ContextMap<MulHash, 20> cm1;
  ContextMap<MulHash, 20> cm2;
  ContextMap<MulHash, 17> cm3;
  ContextMap<MulHash, 18> cm4;
public:
  static constexpr int nProb = decltype(cm1)::nProb * 1;
  static constexpr int nCtx = decltype(cm1)::nCtx * 1;
  static constexpr int CtxShift = 2;

  NewOrders() {

  }

  void predict(uint8_t bit, Prob *pp, Context *pctx) {
    cm1.predict(bit, C, pp, pctx);
    //cm2.predict(bit, C2, pp + decltype(cm1)::nProb * 1, pctx + decltype(cm1)::nCtx * 1);
    //cm3.predict(bit, C3, pp + decltype(cm1)::nProb * 2, pctx + decltype(cm1)::nCtx * 2);
    //cm4.predict(bit, C4, pp + decltype(cm1)::nProb * 3, pctx + decltype(cm1)::nCtx * 3);
    updateContext(bit);

    //*pctx = 0;
  }

  void predict_byte(uint8_t byte, Prob *pp, Context *pctx, size_t pstride, size_t ctxstride) {
    assert(("", counter == 0));

    cm1.predict_byte(byte, C, pp, pctx, pstride, ctxstride);
    //cm2.predict_byte(byte, C2, pp + decltype(cm1)::nProb * 1, pctx + decltype(cm1)::nCtx * 1, pstride, ctxstride);
    //cm3.predict_byte(byte, C3, pp + decltype(cm1)::nProb * 2, pctx + decltype(cm1)::nCtx * 2, pstride, ctxstride);
    //cm4.predict_byte(byte, C4, pp + decltype(cm1)::nProb * 3, pctx + decltype(cm1)::nCtx * 3, pstride, ctxstride);

    //for(int i=0;i<8;i++)
      //pctx[i*ctxstride] = 0;
    updateContextByte(byte);

  }

  void predict_byte_batch(uint8_t *data, size_t size, Prob* pp, Context *pctx, size_t pstride, size_t ctxstride) {
    assert(0);
  }

protected:
  bool updateContext(uint8_t bit) {
    counter++;

    C0 = (C0 << 1) | bit;
    if(counter == 8) {
      updateContextByte(C0);

      C0 = 0;

      counter = 0;
      return true;
    }

    return false;
  }
  void updateContextByte(uint8_t byte) {
    C = ((C << 8) | byte);
    C1 = (C & 0xFF);
    C2 = (C & 0xFFFF);
    C3 = (C & 0xFFFFFF);
    C4 = (C & 0xFFFFFFFF);
  }

  Context get_context() {
    return 0;
  }
};

}