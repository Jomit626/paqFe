#pragma once

#include <cstring>
#include <cmath>

#include "../types.hpp"
#include "StateMap.hpp"
#include "../HashMap.hpp"

#include "TabHash.hpp"
#include "TabHash.h"


namespace paqFe::internal {

uint8_t BitSel(uint8_t x, int i) {
  return (x >> (7 - i)) & 0x1;
}

typedef size_t(*HashFunc)(uint64_t);

template<HashFunc H, int AddrBits>
class ContextMap {
protected:
  union Line {
    struct slot0 {
      State states[3];
      uint8_t c1;
      uint8_t c2;
      uint8_t c3;
      uint8_t cnt;
    } slot0;
    struct slot {
      State states[7];
    } slot;
  };
  //static_assert(sizeof(Line) == 9);

  static constexpr size_t N = 1 << AddrBits;
  AssociativeHashMap<Line, uint8_t, sizeof(Line) * N, 8> hashmap;
  StateMap<1 << 12> sm;
  StateMap<1 << 12> sm2;
  bool first = true;
  uint8_t counter = 0;
  uint32_t C0 = 1;

  Line *l0, *l1, *l2;
  State *active_line;
  int binary_idx = 0;
public:
  static constexpr int nProb = 5;
  static constexpr int nCtx = 0;
  static constexpr int CtxShift = 0;

  State* pState;
  ContextMap() {
    hashmap.find(0, 0, &l0);
    active_line = &l0->slot0.states[0];
    pState = &l0->slot0.states[0];

    for(int j=0;j<nProb;j++)
      pnxt[j] = ProbEven;
  }
  void predict(uint8_t bit, uint64_t ctx, Prob *pp) {
    if(first) first = false;

    sm.update(bit);
    pState->next(bit);

    updateContext(bit, ctx);

    int bpos = counter;
    const uint32_t smask = (uint32_t(0x31031010) >> (bpos << 2)) & 0x0F;
    int pis = smask + (C0 & smask);
    pState = &active_line[pis];
    if(((l0->slot0.c1 >> (8 - bpos)) + 0x100) == C0) {
      const int predictedBit = BitSel(l0->slot0.c1, bpos);
      if(predictedBit) {
        pp[4] = 4090;
      } else {
        pp[4] = 100;
      }
    } else{
      pp[4] = 2048;
    }

    Prob p1;
    int scale = 64;
    sm.predict(*pState, &p1);
    const int st = (LUT.stretch(p1) * scale) >> 8;
    const int contextIsYoung = int(*pState <= 2);
    int n0 = pState->zero_cnt();
    int n1 = pState->one_cnt();
    int bitIsUncertain = n0 != 0 && n1 != 0;
    pp[0] = LUT.squash(st >> contextIsYoung);
    pp[1] = LUT.squash(((p1 - 2048) * scale) >> 9);
    pp[2] = LUT.squash((bitIsUncertain - 1) & st); // when both counts are nonzero add(0) otherwise add(st)
    const int p0 = 4095 - p1;
    pp[3] = LUT.squash((((p1 & (-!n0)) - (p0 & (-!n1))) * scale) >> 10);
    assert(pp[1] > 0 && pp[1] <= 4095);
  }

  Prob pnxt[nProb] = {ProbEven};
  void predict_byte(uint8_t byte, uint64_t ctx, Prob *pp, size_t pstride) {
    for(int i=0;i<8;i++) {
      for(int j=0;j<nProb;j++)
        pp[i * pstride + j] = pnxt[j];
      predict((byte >> (7 - i)) & 0x1, ctx, &pnxt[0]);
    }
  }

  void predict_byte_batch(uint8_t *data, size_t size, Prob* pp, Context *pctx, size_t pstride, size_t ctxstride) {
    assert(0);
  }

protected:
  bool updateContext(uint8_t bit, uint64_t ctx) {
    bool updated = false;
    counter++;

    if(bit)
      binary_idx = 2 * (binary_idx + 1);
    else
      binary_idx = 2 * binary_idx + 1;
    
    //binary_idx %= 9;
    
    C0 = (C0 << 1) | bit;
    uint8_t tag = ctx ^ (ctx >> 8);
    if(counter == 8) {
      if(l0->slot0.c1 == int8_t(C0)) {
        l0->slot0.cnt = 1;
      } else {
        l0->slot0.c2 = l0->slot0.c1;
        l0->slot0.c1 = C0;
        l0->slot0.cnt++;
      }

      updateContextBit8(tag, ctx + C0);

      C0 = 1;
      counter = 0;
      binary_idx = 0;
      updated = true;
    } else if(counter == 6){
      updateContextBit6(tag, ctx + C0);

      //binary_idx = 0;
      updated = true;
    } else if(counter == 3) {
      updateContextBit3(tag, ctx + C0);
      
      //binary_idx = 0;
      updated = true;
    }

    return updated;
  }

  void updateContextBit3(uint8_t tag, uint64_t ctx) {
    hashmap.find(tag, H(ctx), &l1);
    active_line = &l1->slot.states[0];
  }

  void updateContextBit6(uint8_t tag, uint64_t ctx) {
    hashmap.find(tag, H(ctx), &l2);
    active_line = &l2->slot.states[0];
  }

  void updateContextBit8(uint8_t tag, uint64_t ctx) {
    hashmap.find(tag, H(ctx), &l0);
    active_line = &l0->slot0.states[0];
  }
};

}