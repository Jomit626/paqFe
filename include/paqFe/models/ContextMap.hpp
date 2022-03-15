#pragma once

#include <cstring>
#include <cmath>

#include "../types.hpp"
#include "StateMap.hpp"
#include "../HashMap.hpp"

#include "TabHash.hpp"
#include "TabHash.h"


namespace paqFe::internal {

typedef size_t(*HashFunc)(uint64_t);
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

template<HashFunc H, int AddrBits>
class ContextMap {
protected:
  static constexpr size_t N = 1 << AddrBits;
  AssociativeHashMap<Line, uint8_t, sizeof(Line) * N, 16> hashmap;
  StateMap<1 << 12> sm;
  StateMap<1 << 12> sm2;
  bool first = true;
  uint8_t counter = 0;
  uint32_t C0 = 1;

  Line *l0, *l1, *l2;
  State *active_line;
  int binary_idx = 0;
public:
  static constexpr int nProb = 6;
  static constexpr int CtxShift = 0;

  State* pState;
  ContextMap() {
    hashmap.find(0, 0, &l0);
    active_line = &l0->slot0.states[0];
    pState = &l0->slot0.states[0];

    for(int j=0;j<nProb;j++)
      pnxt[j] = ProbEven;
  }
  void predict(uint8_t bit, uint64_t ctx, Prob *pp, int *cnt) {
    if(first) first = false;

    sm.update(bit);
    sm2.update(bit);
    pState->next(bit);

    updateContext(bit, ctx);

    int bpos = counter;
    const uint32_t smask = (uint32_t(0x31031010) >> (bpos << 2)) & 0x0F;
    const int bp = (0xFEA4U >> (bpos << 1U)) & 3U; // {0}->0  {1}->1  {2,3,4}->2  {5,6,7}->3
    int pis = smask + (C0 & smask);
    pState = &active_line[pis];
    if(((l0->slot0.c1 + 0x100) >> (8 - bpos)) == C0) {
      const int predictedBit = BitSel(l0->slot0.c1, bpos);
      const int byte1IsUncertain = static_cast<const int>(l0->slot0.c1 != l0->slot0.c2);
      const int runCount = l0->slot0.cnt;
      int c = runCount << 4U | bp << 2U | byte1IsUncertain << 1 | predictedBit;
      sm2.predict(c, &pp[5]);
    } else{
      sm2.predict(0, &pp[5]);
    }
  
    Prob p1;
    sm.predict(*pState, &p1);
    if(*pState != 0) {
      int scale = 64;
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
      *cnt = *cnt + 1;
    } else {
      pp[0] = pp[1] = pp[2] = pp[3] = pp[5];
    }
    pp[4] = p1;
  }

  Prob pnxt[nProb] = {ProbEven};
  void predict_byte(uint8_t byte, uint64_t ctx, Prob *pp, size_t pstride, int cnt[8]) {
    for(int i=0;i<8;i++) {
      for(int j=0;j<nProb;j++)
        pp[i * pstride + j] = pnxt[j];
      predict((byte >> (7 - i)) & 0x1, ctx, &pnxt[0], &cnt[i]);
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
      l0->slot0.c2 = l0->slot0.c1;
      if(l0->slot0.c1 == int8_t(C0)) {
        l0->slot0.cnt++;
      } else {
        l0->slot0.c1 = C0;
        l0->slot0.cnt = 1;
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