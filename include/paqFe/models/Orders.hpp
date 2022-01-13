#pragma once

#include <cstring>

#include "../types.hpp"
#include "StateMap.hpp"

namespace paqFe::internal {

struct Line {
  State states[15];
  uint8_t checksum;
};

template< size_t O2Size = 1ul << 16,
          size_t O3Size = 1ul << 19,
          size_t O4Size = 1ul << 21>
class Orders {
protected:
  static constexpr size_t O1Size = 1ul << 12;
#define DECARE_SIZE(name) \
  static constexpr size_t name##SizeMask = name##Size - 1;  \
  static_assert(isPow2(name##Size));  \

  DECARE_SIZE(O1)
  DECARE_SIZE(O2)
  DECARE_SIZE(O3)
  DECARE_SIZE(O4)

#undef DECARE_SIZE

  Line o1_lines[O1Size];
  Line o2_lines[O2Size];
  Line o3_lines[O3Size];
  Line o4_lines[O4Size];

  StaticStateMap sm;

  //
  bool first = true;

  // bit indicators in bitwise prediction
  int counter = 0;
  int binary_idx = 0;

  Line* o1_line = o1_lines;
  bool o1_hit = false;
  Line* o2_line = o2_lines;
  bool o2_hit = false;
  Line* o3_line = o3_lines;
  bool o3_hit = false;
  Line* o4_line = o4_lines;
  bool o4_hit = false;

  // bytewise context
  uint8_t C0 = 0; // current byte
  uint32_t C1 = 0;     // prev byte
  uint32_t C2 = 0;
  uint32_t C3 = 0;
  uint32_t C4 = 0;
  uint32_t C = 0;

public:
  static constexpr int OutputCnt = 4;

  Orders() {
    std::memset(o1_lines, 0X00, sizeof(o1_lines));
    std::memset(o2_lines, 0X00, sizeof(o2_lines));
    std::memset(o3_lines, 0X00, sizeof(o3_lines));
    std::memset(o4_lines, 0X00, sizeof(o4_lines));
  }

  void predict(uint8_t bit, Prob *pp, Context *pctx) {
    if(first) first = false;

    // update states
    o1_line->states[binary_idx].next(bit);
    o2_line->states[binary_idx].next(bit);
    o3_line->states[binary_idx].next(bit);
    o4_line->states[binary_idx].next(bit);

    if(updateContext(bit))
      selectLines();

    // do prediction
    pp[0] = sm.predict(o1_line->states[binary_idx]);
    pp[1] = sm.predict(o2_line->states[binary_idx]);
    pp[2] = sm.predict(o3_line->states[binary_idx]);
    pp[3] = sm.predict(o4_line->states[binary_idx]);
    
    pctx[0] = C1;
    pctx[1] = o2_hit ? 1 : 0;
    pctx[2] = o3_hit ? 1 : 0;
    pctx[3] = o4_hit ? 1 : 0;
  }

  void predict_byte(uint8_t byte, Prob *pp, Context *pctx, size_t stride = OutputCnt) {
    assert(("", counter == 0));
    
    uint8_t nibble0 = byte >> 4;
    uint8_t nibble1 = byte & 0xF;

    
    predict_nibble(nibble0, o1_line, true, pp, pctx, stride);
    predict_nibble(nibble0, o2_line, o2_hit, pp + 1, pctx + 1, stride);
    predict_nibble(nibble0, o3_line, o3_hit, pp + 2, pctx + 2, stride);
    predict_nibble(nibble0, o4_line, o4_hit, pp + 3, pctx + 3, stride);
    for(int i=0;i<4;i++) {
      pctx[i*stride] = C1;
    }
    updateContextNibble1(nibble0);
    selectLines();

    //predict_nibble(nibble1, o1_line, pp + 4, 1);
    predict_nibble(nibble1, o1_line, true, pp + 4 * stride,     pctx + 4 * stride, stride);
    predict_nibble(nibble1, o2_line, o2_hit, pp + 4 * stride + 1, pctx + 4 * stride + 1, stride);
    predict_nibble(nibble1, o3_line, o3_hit, pp + 4 * stride + 2, pctx + 4 * stride + 2, stride);
    predict_nibble(nibble1, o4_line, o4_hit, pp + 4 * stride + 3, pctx + 4 * stride + 3, stride);
    for(int i=0;i<4;i++) {
      pctx[(i + 4)*stride] = C1;
    }
    updateContextNibble0(nibble1);
    selectLines();

    if(first) {
      pp[0] = ProbEven;
      pp[1] = ProbEven;
      pp[2] = ProbEven;
      pp[3] = ProbEven;
      pctx[0] = 0;
      pctx[1] = 0;
      pctx[2] = 0;
      pctx[3] = 0;
      first = false;
    }
  }

  void predict_byte_batch(uint8_t *data, size_t size, Prob* pp, Context *pctx, size_t stride = OutputCnt) {
    assert(0);
  }

protected:
  bool updateContext(uint8_t bit) {
    counter++;

    if(bit)
      binary_idx = 2 * (binary_idx + 1);
    else
      binary_idx = 2 * binary_idx + 1;

    C0 = (C0 << 1) | bit;
    uint8_t nibble = C0 & 0xF;
    if(counter == 8) {
      updateContextNibble0(nibble);

      C0 = 0;

      counter = 0;
      binary_idx = 0;
      return true;
    } else if(counter == 4){
      updateContextNibble1(nibble);

      binary_idx = 0;
      return true;
    }

    return false;
  }

  bool updateContextNibble0(uint8_t nibble) {
    C = ((C << 4) | nibble);
    C1 = C & 0xFFF;
    C2 = (C & 0xFFFF) << 5;
    C3 = (C << 8) * 3;
    C4 = C * 5;
    return true;
  }

  bool updateContextNibble1(uint8_t nibble) {
    C = ((C << 4) | nibble);
    C1 = C1 + nibble + 16;
    C2 = C2 + nibble + 16;
    C3 = C3 + nibble + 16;
    C4 = C4 + nibble + 16;

    return true;
  }

  void selectLines() {
    o1_line = selLine(o1_lines, C1, C1 & O1SizeMask, &o1_hit);
    o2_line = selLine(o2_lines, C2, hash(C2) & O2SizeMask, &o2_hit);
    o3_line = selLine(o3_lines, C3, hash(C3) & O3SizeMask, &o3_hit);
    o4_line = selLine(o4_lines, C4, hash(C4) & O4SizeMask, &o4_hit);
  }

  uint32_t hash(uint32_t val) {
    return (val * 123431523 + val - 3) ^ (val << 3);
  }

  Line* selLine(Line* lines, uint32_t val, uint32_t hashval, bool *hit) {
    Line* l = &lines[hashval];

    if(l->checksum != uint8_t(val)) {
      std::memset(l, 0x00, sizeof(Line));
      l->checksum = uint8_t(val);

      *hit = false;
    } else {
      *hit = true;
    }

    return l;
  }

  void predict_nibble(uint8_t nibble, Line *l, bool hit, Prob* pp, Context *pctx, size_t stride = OutputCnt) {
    int idx0 = 1 + (nibble >> 3);
    int idx1 = 3 + (nibble >> 2);
    int idx2 = 7 + (nibble >> 1);

    State* states = l->states;

    pp[0] = sm[states[0]];
    pctx[0] = hit ? 1 : 0;
    states[0].next((nibble >> 3) & 0x1);

    pp[1 * stride] = sm[states[idx0]];
    pctx[1 * stride] = hit ? 1 : 0;
    states[idx0].next((nibble >> 2) & 0x1);

    pp[2 * stride] = sm[states[idx1]];
    pctx[2 * stride] = hit ? 1 : 0;
    states[idx1].next((nibble >> 1) & 0x1);

    pp[3 * stride] = sm[states[idx2]];
    pctx[3 * stride] = hit ? 1 : 0;
    states[idx2].next((nibble >> 0) & 0x1);
  }
};

}