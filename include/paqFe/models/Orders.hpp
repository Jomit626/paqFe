#pragma once

#include <cstring>
#include <cmath>

#include "../types.hpp"
#include "StateMap.hpp"

#include "TabHash.hpp"
#include "TabHash.h"

namespace paqFe::internal
{
#define DOT ,
#define ORDERS(OP, SEP)        \
  OP(1, 0)                     \
  SEP                          \
      OP(2, 1) SEP             \
          OP(3, 2) SEP         \
              OP(4, 3) SEP     \
                  OP(5, 4) SEP \
                  OP(Word, 5)

#define DECARE_ADDRWIDTH(order, id) size_t O##order##AddrWidth

  template <
      ORDERS(DECARE_ADDRWIDTH, DOT)>
#undef DECARE_ADDRWIDTH
  class Orders
  {
  protected:
    struct Line
    {
      State states[15];
      uint8_t checksum;
    };

#define DECARE_VAR(order, id)                                              \
  static constexpr size_t O##order##AddrWidth0 = O##order##AddrWidth;      \
  static constexpr size_t O##order##LineSize = 1ul << O##order##AddrWidth; \
  static constexpr size_t O##order##Mask = O##order##LineSize - 1;         \
  static_assert(isPow2(sizeof(Line)));                                     \
                                                                           \
  Line O##order##lines[O##order##LineSize];                                \
  Line *O##order##line = O##order##lines;                                  \
  bool O##order##hit = true;                                               \
                                                                           \
  uint64_t C##order = 0;                                                   \
  uint64_t H##order = 0;

    ORDERS(DECARE_VAR, )
#undef DECARE_VAR

    bool first = true;

    // bit indicators in bitwise prediction
    int counter = 0;
    int binary_idx = 0;

    // bytewise context
    uint8_t C0 = 0; // current byte
    uint64_t C = 0;
    int salt = 0;

    Context MixCtx[8] = {};

  public:
    static constexpr int nProbPerOrder = 1;
    static constexpr int nProb = 6 * nProbPerOrder;
    static constexpr int nCtx = 6;

    Orders()
    {
#define MEMSET_ZERO(order, id) std::memset(O##order##lines, 0X00, sizeof(O##order##lines));
      ORDERS(MEMSET_ZERO, )
#undef MEMSET_ZERO
    }

    void predict(uint8_t bit, Prob *pp, Context *pctx)
    {
      if (first)
        first = false;

#define UPDATE_STATES(order, id) O##order##line->states[binary_idx].next(bit);
      ORDERS(UPDATE_STATES, )
#undef UPDATE_STATES

      updateContext(bit);

#define PREDICTION(order, id) state2prob(O##order##line->states[binary_idx], &pp[id * nProbPerOrder]);
      ORDERS(PREDICTION, )
#undef PREDICTION

      setContext(pctx);
    }

    void predict_byte(uint8_t byte, Prob *pp, Context *pctx, size_t pstride, size_t ctxstride)
    {
      assert(("", counter == 0));

      uint8_t nibble0 = byte >> 4;
      uint8_t nibble1 = byte & 0xF;

#define PREDICTI_NIBBLE(order, id) \
  predict_nibble(nibble0, O##order##line, pp + id * nProbPerOrder, pstride);

      ORDERS(PREDICTI_NIBBLE, )
#undef PREDICTI_NIBBLE

      for (int i = 0; i < 4; i++)
        setContext(&pctx[i * ctxstride]);

      updateContextNibble1(nibble0);

#define PREDICTI_NIBBLE(order, id) \
  predict_nibble(nibble1, O##order##line, pp + 4 * pstride + id * nProbPerOrder, pstride);

      ORDERS(PREDICTI_NIBBLE, )
#undef PREDICTI_NIBBLE

      for (int i = 4; i < 8; i++)
        setContext(&pctx[i * ctxstride]);

      updateContextNibble0(nibble1, byte);

      if (first)
      {
        for (int i = 0; i < nProb; i++)
          pp[i] = ProbEven;
        for (int i = 0; i < nCtx; i++)
          pctx[i] = 0;
        first = false;
      }
    }

    void predict_byte_batch(uint8_t *data, size_t size, Prob *pp, Context *pctx, size_t pstride, size_t ctxstride)
    {
      assert(0);
    }

  protected:
    bool updateContext(uint8_t bit)
    {
      counter++;

      if (bit)
        binary_idx = 2 * (binary_idx + 1);
      else
        binary_idx = 2 * binary_idx + 1;

      C0 = (C0 << 1) | bit;
      if (counter == 8)
      {
        uint8_t nibble = C0 & 0xF;
        uint8_t byte = C0 & 0xFF;
        updateContextNibble0(nibble, byte);

        C0 = 0;

        counter = 0;
        binary_idx = 0;
        return true;
      }
      else if (counter == 4)
      {
        uint8_t nibble = C0 & 0xF;
        updateContextNibble1(nibble);

        binary_idx = 0;
        return true;
      }

      return false;
    }

    bool updateContextNibble0(uint8_t nibble, uint8_t byte)
    {
      salt += 1;

      C = ((C << 4) | nibble);
      C1 = (C & 0xFF) << 5;
      C2 = (C & 0xFFFF) << 5;
      C3 = (C & 0xFFFFFF) << 5;
      C4 = (C & 0xFFFFFFFF) << 5;
      C5 = (C & 0xFFFFFFFFFF) << 5;

      bool isWord = false;
      if (byte >= 65 && byte <= 90)
      {
        byte += 32;
        isWord = true;
      }
      else if ((byte >= 97 && byte <= 122))
      {
        isWord = true;
      }

      if (isWord)
      {
        CWord = (CWord ^ byte) << 5;
      }
      else
      {
        CWord = 0;
      }
      updateHash();
      selectLines();

      return true;
    }

    bool updateContextNibble1(uint8_t nibble)
    {
      C = ((C << 4) | nibble);
#define UPDATE_CONTEXT(order, id) C##order = C##order + nibble + 16;
      ORDERS(UPDATE_CONTEXT, )
#undef UPDATE_CONTEXT
      updateHash();
      selectLines();

      return true;
    }

    void updateHash()
    {
      H1 = C1 & O1Mask;
      H2 = tab_hashing<21, O2AddrWidth>(O2HashTab, C2) & O2Mask;
      H3 = tab_hashing<29, O3AddrWidth>(O3HashTab, C3) & O3Mask;
      H4 = tab_hashing<37, O4AddrWidth>(O4HashTab, C4) & O4Mask;
      H5 = tab_hashing<45, O5AddrWidth>(O5HashTab, C5) & O5Mask;
      HWord = (CWord ^ (CWord >> 32) ^ (CWord >> 16)) & OWordMask;
    }

    uint32_t prevHitVec = 0;
    uint32_t prevHitVec2 = 0;
    int wordRunLevel = 0;
    int prevWordRunLevel = 0;
    int highRunLevel = 0;
    void selectLines()
    {
#define SELECT_LINE(order, id) O##order##line = selLine(O##order##lines, C##order, H##order, &O##order##hit);
      ORDERS(SELECT_LINE, )
#undef SELECT_LINE

      wordRunLevel = (wordRunLevel << 1) & 0x3F;
      if (OWordhit && HWord != 0)
      {
        wordRunLevel |= 1;
      }

      uint32_t hitVec = (O1hit << 5) | (O2hit << 4) | (O3hit << 3) | (O4hit << 2) | (O5hit << 1) | (OWordhit << 0);
      if (O5hit)
      {
        if (highRunLevel < 31)
        {
          highRunLevel += 1;
        }
      }
      else
      {
        highRunLevel = 0;
      }
      int n = 0;
      MixCtx[n++] = (salt & 0x3) | (hitVec << 2);
      MixCtx[n++] = (salt & 0x3) | ((prevHitVec & hitVec) << 2);
      MixCtx[n++] = (salt & 0x3) | ((prevHitVec2 & hitVec) << 2);
      MixCtx[n++] = (salt & 0x3) | (wordRunLevel << 2);
      MixCtx[n++] = (salt & 0x3) | (prevWordRunLevel << 2);
      MixCtx[n++] = (salt & 0x3) | (highRunLevel << 2);

      prevWordRunLevel = wordRunLevel;
      prevHitVec2 = prevHitVec;
      prevHitVec = hitVec;
    }

    Line *selLine(Line *lines, uint32_t val, uint32_t hashval, bool *hit)
    {
      Line *l = &lines[hashval];

      if (l->checksum != uint8_t(val))
      {
        std::memset(l, 0x00, sizeof(Line));
        l->checksum = uint8_t(val);

        *hit = false;
      }
      else
      {
        *hit = true;
      }

      return l;
    }

    void state2prob(State s, Prob *pp)
    {
      Prob p1 = pp[0] = StaticStateMap::map[s];
      Prob p0 = 4096 - p1;
      if constexpr (nProbPerOrder > 1)
      {
        if (s.one_cnt() || s.zero_cnt())
        {
          pp[1] = p1;
        }
        else
        {
          pp[1] = ProbEven;
        }
      }
    }

    void predict_nibble(uint8_t nibble, Line *l, Prob *pp, size_t stride)
    {
      int idx0 = 1 + (nibble >> 3);
      int idx1 = 3 + (nibble >> 2);
      int idx2 = 7 + (nibble >> 1);

      State *states = l->states;

      state2prob(states[0], &pp[stride * 0]);
      states[0].next((nibble >> 3) & 0x1);

      state2prob(states[idx0], &pp[stride * 1]);
      states[idx0].next((nibble >> 2) & 0x1);

      state2prob(states[idx1], &pp[stride * 2]);
      states[idx1].next((nibble >> 1) & 0x1);

      state2prob(states[idx2], &pp[stride * 3]);
      states[idx2].next((nibble >> 0) & 0x1);
    }

    void setContext(Context *pctx)
    {
      for (int i = 0; i < nCtx; i++)
        pctx[i] = MixCtx[i];
    }
  };

  using OrdersDefault = Orders<12, 16, 16, 17, 17, 17>;

}
