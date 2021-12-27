#pragma once

#include <cstring>

#include "../types.hpp"
#include "StateMap.hpp"

namespace paqFe::internal {

struct Line {
  State states[15];
  uint8_t checksum;
};

class Orders {
protected:
#define DECARE_SIZE(name, size) \
  static constexpr size_t name##Size = size;  \
  static constexpr size_t name##SizeMask = name##Size - 1;  \
  static_assert(isPow2(name##Size));  \

  DECARE_SIZE(O1, 1ul << 12)
  DECARE_SIZE(O2, 1ul << 20)
  DECARE_SIZE(O3, 1ul << 21)
  DECARE_SIZE(O4, 1ul << 22)

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
  Line* o2_line = o2_lines;
  Line* o3_line = o3_lines;
  Line* o4_line = o4_lines;

  // bytewise context
  uint8_t C0 = 0; // current byte
  uint32_t C1 = 0;     // prev byte
  uint32_t C2 = 0;
  uint32_t C3 = 0;
  uint32_t C4 = 0;

public:
  static constexpr int n_output = 4;

  Orders() {
    std::memset(o1_lines, 0X00, sizeof(o1_lines));
    std::memset(o2_lines, 0X00, sizeof(o2_lines));
    std::memset(o3_lines, 0X00, sizeof(o3_lines));
    std::memset(o4_lines, 0X00, sizeof(o4_lines));
  }

  void predict(uint8_t bit, Prob *pp) {
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
  }

  void predict_byte(uint8_t byte, Prob *pp, size_t stride = n_output) {
    assert(("", counter == 0));
    assert(stride == 4);
    uint8_t nibble0 = byte >> 4;
    uint8_t nibble1 = byte & 0xF;

    //predict_nibble(nibble0, o1_line, pp, 1);
    predict_nibble(nibble0, o1_line, pp, stride);
    predict_nibble(nibble0, o2_line, pp + 1, stride);
    predict_nibble(nibble0, o3_line, pp + 2, stride);
    predict_nibble(nibble0, o4_line, pp + 3, stride);
    updateContextNibble(nibble0);
    selectLines();

    //predict_nibble(nibble1, o1_line, pp + 4, 1);
    predict_nibble(nibble1, o1_line, pp + 4 * stride, stride);
    predict_nibble(nibble1, o2_line, pp + 4 * stride + 1, stride);
    predict_nibble(nibble1, o3_line, pp + 4 * stride + 2, stride);
    predict_nibble(nibble1, o4_line, pp + 4 * stride + 3, stride);
    updateContextNibble(nibble1);
    selectLines();

    if(first) {
      pp[0] = ProbEven;
      pp[1] = ProbEven;
      pp[2] = ProbEven;
      pp[3] = ProbEven;
      first = false;
    }
  }

  void predict_byte_batch(uint8_t *data, size_t size, Prob* pp) {
    assert(0);
  }

  Context getContext() {
    return 0;
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
      updateContextNibble(nibble);

      C0 = 0;

      counter = 0;
      binary_idx = 0;
      return true;
    } else if(counter == 4){
      updateContextNibble(nibble);

      binary_idx = 0;
      return true;
    }

    return false;
  }

  bool updateContextNibble(uint8_t nibble) {
    C1 = ((C1 << 4) | nibble) & O1SizeMask;
    C2 = ((C2 << 4) | nibble) & O2SizeMask;
    C3 = ((C3 << 4) | nibble) & O3SizeMask;
    C4 = ((C4 << 4) | nibble) & O4SizeMask;

    return true;
  }

  void selectLines() {
    o1_line = selLine(o1_lines, O1SizeMask ,C1);
    o2_line = selLine(o2_lines, O2SizeMask ,C2);
    o3_line = selLine(o3_lines, O3SizeMask ,C3);
    o4_line = selLine(o4_lines, O4SizeMask ,C4);
  }

  uint32_t hash(uint32_t val) {
    return val ^ (val << 3);
  }

  Line* selLine(Line* lines, size_t mask, uint32_t val) {
    uint32_t hashval = hash(val) & mask;

    Line* l = &lines[hashval];

    if(l->checksum != uint8_t(val)) {
      std::memset(l, 0x00, sizeof(Line));
      l->checksum = uint8_t(val);
    }

    return l;
  }

  void predict_nibble(uint8_t nibble, Line *l, Prob* pp, size_t stride = n_output) {
    int idx0 = 1 + (nibble >> 3);
    int idx1 = 3 + (nibble >> 2);
    int idx2 = 7 + (nibble >> 1);

    State* states = l->states;

    pp[0] = sm[states[0]];
    states[0].next((nibble >> 3) & 0x1);

    pp[1 * stride] = sm[states[idx0]];
    states[idx0].next((nibble >> 2) & 0x1);

    pp[2 * stride] = sm[states[idx1]];
    states[idx1].next((nibble >> 1) & 0x1);

    pp[3 * stride] = sm[states[idx2]];
    states[idx2].next((nibble >> 0) & 0x1);
  }
};

}