#pragma once

#include <cstring>
#include <cmath>

#include "../types.hpp"
#include "StateMap.hpp"
#include "ContextMap2.hpp"

namespace paqFe::internal {

template<
  int C1AddrBits = 12,
  int C2AddrBits = 16,
  int C3AddrBits = 16,
  int C4AddrBits = 16,
  int C5AddrBits = 16,
  int C6AddrBits = 17,
  int C7AddrBits = 17,
  int C9AddrBits = 17,
  int C12AddrBits = 17
>
class NormalModel {
protected:
  static size_t MulHash(uint64_t val) {
    return (val * 	10000000807) ^ ((val >> 32) * 123456);
  }

  int counter = 0;
  uint32_t C0 = 0; // current byte
  uint64_t C = 0;
  uint64_t ctx[15];
  
  ContextMap<MulHash, C1AddrBits> cm1;
  ContextMap<MulHash, C2AddrBits> cm2;
  ContextMap<MulHash, C3AddrBits> cm3;
  ContextMap<MulHash, C4AddrBits> cm4;
  ContextMap<MulHash, C5AddrBits> cm5;
  ContextMap<MulHash, C6AddrBits> cm6;
  ContextMap<MulHash, C7AddrBits> cm7;
  ContextMap<MulHash, C9AddrBits> cm9;
  ContextMap<MulHash, C12AddrBits> cm14;

public:
  static constexpr int nProb = decltype(cm1)::nProb * 9;
  static constexpr int nCtx = 1;

  NormalModel() {
    for(int i=0;i<15;i++) {
      ctx[i] = 0;
    }
  }

  void predict(uint8_t bit, Prob *pp, Context *pctx) {
    int order = 0;
    
    updateContext(bit);

    cm1.predict(bit, ctx[1], pp, &order); pp += decltype(cm1)::nProb;
    cm2.predict(bit, ctx[2], pp, &order); pp += decltype(cm2)::nProb;
    cm3.predict(bit, ctx[3], pp, &order); pp += decltype(cm3)::nProb;
    cm4.predict(bit, ctx[4], pp, &order); pp += decltype(cm4)::nProb;
    cm5.predict(bit, ctx[5], pp, &order); pp += decltype(cm5)::nProb;
    cm6.predict(bit, ctx[6], pp, &order); pp += decltype(cm6)::nProb;
    cm7.predict(bit, ctx[7], pp, &order); pp += decltype(cm7)::nProb;
    cm9.predict(bit, ctx[9], pp, &order); pp += decltype(cm9)::nProb;
    cm14.predict(bit, ctx[14], pp, &order); pp += decltype(cm14)::nProb;
    
    // order <= 9
    int bpos = counter;
    uint32_t c0 = C0;
    uint32_t c1 = C & 0xFF;
    uint32_t c2 = (C >> 8) & 0xFF;
    uint32_t c3 = (C >> 16) & 0xFF;
    uint32_t c4 = C;
    uint32_t c;
    order = max(order - 2, 0);
    pctx[0] = (order << 3) | counter;
    /*
    pctx[1] = 8 + (
      (c1 & 0xFF) | 
      (static_cast<int>(bpos > 5) << 8U) | 
      (static_cast<int>(((C0 & ((1U << bpos) - 1)) == 0) || (C0 == ((2 << bpos) - 1))) << 9U));
    pctx[2] = c0 & 0xFF;
    pctx[3] = order |
              ((c4 >> 6U) & 3U) << 3U |
              static_cast<int>(bpos == 0) << 5U |
              static_cast<int>(c1 == c2) << 6U;
    pctx[4] = c2;
    pctx[5] = c3;

    if( bpos != 0 ) {
      c = C0 << (8 - bpos);
      if( bpos == 1 ) {
        c |= c3 >> 1U;
      }
      c = std::min(bpos, 5) << 8U | c1 >> 5U | (c2 >> 5U) << 3U | (c & 192U);
    } else {
      c = c3 >> 7U | (c4 >> 31U) << 1U | (c2 >> 6U) << 2U | (c1 & 240U);
    }
    pctx[6] = c;
    */
  }

  /*
  void predict_byte(uint8_t byte, Prob *pp, Context *pctx, size_t pstride, size_t ctxstride) {
    assert(("", counter == 0));

    cm1.predict_byte(byte, ctx[1], pp, pstride); pp += decltype(cm1)::nProb;
    cm2.predict_byte(byte, ctx[2], pp, pstride); pp += decltype(cm2)::nProb;
    cm3.predict_byte(byte, ctx[3], pp, pstride); pp += decltype(cm3)::nProb;
    cm4.predict_byte(byte, ctx[4], pp, pstride); pp += decltype(cm4)::nProb;
    cm5.predict_byte(byte, ctx[5], pp, pstride); pp += decltype(cm5)::nProb;
    cm6.predict_byte(byte, ctx[6], pp, pstride); pp += decltype(cm6)::nProb;
    cm7.predict_byte(byte, ctx[7], pp, pstride); pp += decltype(cm7)::nProb;
    cm9.predict_byte(byte, ctx[9], pp, pstride); pp += decltype(cm9)::nProb;
    cm14.predict_byte(byte, C, ctx[14], pstride); pp += decltype(cm14)::nProb;

    updateContextByte(byte);
  }*/

protected:
  void updateContext(uint8_t bit) {
    counter++;

    C0 = (C0 << 1) | bit;
    if(counter == 8) {
      updateContextByte(C0);

      C0 = 1;
      counter = 0;
    }
  }

  uint64_t hash(const uint64_t x0) {
  return (x0 + 1) * UINT64_C(0x9E3779B97F4A7C15);
  }

  void updateContextByte(uint8_t byte) {
    C = (C << 8) | byte;
    for(int i = 14; i > 0; --i) {
      ctx[i] = hash(ctx[i - 1] + (byte + (i << 10U)));
    }
  }
};

}