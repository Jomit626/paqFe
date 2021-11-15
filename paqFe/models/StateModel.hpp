#pragma once

#include <cstring>
#include "../types.hpp"

namespace paqFe {

class State {
  static uint8_t table[256][2];

  uint8_t s;
public:

  void update(uint8_t bit){
    s = table[s][bit];
  }

  operator uint8_t() {
    return s;
  }
};

template<size_t size>
class StateHashTable {

  static_assert(isPow2(size), "size of StateHashTable must be a pow of 2");

  static constexpr size_t hashval_mask = size - 1;

  class StateHashTableLine;
  StateHashTableLine table[size];

public:
  class StateHashTableLine {
    uint8_t checksum;
    State states[15];
  };

  StateHashTable() {
    memset(table, 0x00, sizeof(table));
  }

  StateHashTableLines* operator[](size_t hashval) {
    return &table[hashval & hashval_mask];
  }

  
};

template<size_t size>
class ContextModel {

  uint8_t C0; // previous bits with leading 1
  uint8_t C4; // previous 4 C0

  StateHashTable<size> hashtable;

  Context Ctxes[8] = {0};
  StateHashTable<size>::StateHashTableLine* lines[8];

  int counter = 0;

public:
  static constexpr int n_output = 2;

  ContextModel() {

  }

  void predict(uint8_t bit, Prob* pp) {
    counter++;

    C0 = (C0 << 1) | bit;
    if(C0 & 0x100) {
      C0 = C0 & 0xFF;
      C4 = (C4 << 8) | C0;

      Ctxes[0] = C0;
      Ctxes[1] = C4 & 0xFFFF;
      Ctxes[2] = C4 & 0xFFFFFF;
      Ctxes[3] = C4;

      Ctxes[3] = C4;
    }  
  }

  Context getContext(int n = 0){
    return Ctxes[n];
  }
};

}