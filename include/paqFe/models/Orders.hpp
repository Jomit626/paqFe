#pragma once

#include <cstring>

#include "../types.hpp"
#include "StateMap.hpp"

namespace paqFe::internal::order {

struct Line {
  State states[15];
  uint8_t checksum;
};

class Order1 {
  Line o1_lines[(1 << 12)];

  StaticStateMap sm;

  // bit indicators in bitwise prediction
  int counter = 0;
  int binary_idx = 0;
  Line* o1_line = o1_lines;

  // bytewise context
  uint8_t C0 = 0; // current byte
  uint32_t C1 = 0;     // prev byte

public:
  static constexpr int OutputCnt = 1;

  Order1() {
    std::memset(o1_lines, 0X00, sizeof(o1_lines));
  }

  void predict(uint8_t bit, Prob *pp) {
    // update states
    o1_line->states[binary_idx].next(bit);

    if(updateContext(bit))
      selectLines();

    // do prediction
    pp[0] = sm.predict(o1_line->states[binary_idx]);
  }

  void predict_byte(uint8_t byte, Prob *pp) {
    assert(("", counter == 0));
    uint8_t nibble = byte >> 4;

    int idx0 = 1 + (nibble >> 3);
    int idx1 = 3 + (nibble >> 2);
    int idx2 = 7 + (nibble >> 1);

    State* states = o1_line->states;
    states[0].next((nibble >> 3) & 0x1);

    pp[0] = sm.predict(states[idx0]);
    states[idx0].next((nibble >> 2) & 0x1);

    pp[1] = sm.predict(states[idx1]);
    states[idx1].next((nibble >> 1) & 0x1);

    pp[2] = sm.predict(states[idx2]);
    states[idx2].next((nibble >> 0) & 0x1);

    C1 = (C1 << 4) | nibble;
    nibble = byte & 0xF;

    selectLines();
    states = o1_line->states;

    idx0 = 1 + (nibble >> 3);
    idx1 = 3 + (nibble >> 2);
    idx2 = 7 + (nibble >> 1);

    pp[3] = sm.predict(states[0]);
    states[0].next((nibble >> 3) & 0x1);

    pp[4] = sm.predict(states[idx0]);
    states[idx0].next((nibble >> 2) & 0x1);

    pp[5] = sm.predict(states[idx1]);
    states[idx1].next((nibble >> 1) & 0x1);

    pp[6] = sm.predict(states[idx2]);
    states[idx2].next((nibble >> 0) & 0x1);


    C1 = byte;
    selectLines();

    pp[7] = sm.predict(o1_line->states[0]);
  }

  void predict_byte_batch(uint8_t *data, size_t size, Prob* pp) {
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
    if(counter == 8) {

      C1 = C0;
      C0 = 0;

      counter = 0;
      binary_idx = 0;
      return true;
    } else if(counter == 4){
      
      C1 = (C1 << 4) | C0;
      binary_idx = 0;
      return true;
    }

    return false;
  }

  void selectLines() {
    o1_line = &o1_lines[C1];
    
    if(o1_line->checksum != uint8_t(C1)) {
      std::memset(o1_line, 0x00, sizeof(Line));
      o1_line->checksum = uint8_t(C1);
    }
  }
};

}