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

  ContextMap<MulHash, 12> cm1;
  ContextMap<MulHash, 21> cm2;
  ContextMap<MulHash, 21> cm3;
  ContextMap<MulHash, 21> cm4;
public:
  static constexpr int nProb = 4 * 5;
  static constexpr int nCtx = 1;
  static constexpr int CtxShift = 2;

  NewOrders() {

  }

  void predict(uint8_t bit, Prob *pp, Context *pctx) {
    cm1.predict(bit, C1, pp);
    cm2.predict(bit, C2, pp + 1 * 5);
    cm3.predict(bit, C3, pp + 2 * 5);
    cm4.predict(bit, C4, pp + 3 * 5);

    *pctx = 0;
  }

  void predict_byte(uint8_t byte, Prob *pp, Context *pctx, size_t pstride, size_t ctxstride) {
    assert(("", counter == 0));

    cm1.predict_byte(byte, C1, pp, pstride);
    cm2.predict_byte(byte, C2, pp + 1 * 5, pstride);
    cm3.predict_byte(byte, C3, pp + 2 * 5, pstride);
    cm4.predict_byte(byte, C4, pp + 3 * 5, pstride);

    for(int i=0;i<8;i++)
      pctx[i*ctxstride] = 0;
  }

  void predict_byte_batch(uint8_t *data, size_t size, Prob* pp, Context *pctx, size_t pstride, size_t ctxstride) {
    assert(0);
  }

protected:
  bool updateContext(uint8_t bit) {
    counter++;

    C0 = (C0 << 1) | bit;
    uint8_t nibble = C0 & 0xF;
    if(counter == 8) {
      updateContextNibble0(nibble);

      C0 = 0;

      counter = 0;
      return true;
    } else if(counter == 4){
      updateContextNibble1(nibble);

      return true;
    }

    return false;
  }

  bool updateContextNibble0(uint8_t nibble) {
    C = ((C << 4) | nibble);
    C1 = (C & 0xFF);
    C2 = (C & 0xFFFF);
    C3 = (C & 0xFFFFFF);
    C4 = (C & 0xFFFFFFFF);
    return true;
  }

  bool updateContextNibble1(uint8_t nibble) {
    C = ((C << 4) | nibble);

    return true;
  }

  Context get_context() {
    return 0;
  }
};

}