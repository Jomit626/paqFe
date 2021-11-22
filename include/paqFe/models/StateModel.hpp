#pragma once

#include <cstring>
#include "../types.hpp"
#include "StateMap.hpp"

namespace paqFe {

struct StateHashTableLine {
  State states[31];
  uint8_t checksum;
};

template<size_t size>
class StateHashTable {
  static_assert(isPow2(size), "size of StateHashTable must be a pow of 2");

  static constexpr size_t hashval_mask = size - 1;

public:

  StateHashTableLine table[size];
  StateHashTable() {
    memset(table, 0x00, sizeof(table));
  }

  StateHashTableLine* operator[](uint32_t key) {
    size_t hashval = hash(key);
    StateHashTableLine* line = &table[hashval];

    if(line->checksum != uint8_t(key)) {
      line->checksum = uint8_t(key);
      memset(line->states, 0x00, sizeof(line->states));
    }

    return line;
  }

  size_t hash(uint32_t key) {
    key*=123456791;
    key=key<<16|key>>16;
    key*=234567891;
    return key & hashval_mask;
  }
  
};

template<size_t size>
class ContextModel {

  uint8_t C0 = 0;
  uint32_t C4 = 0;

  size_t order1 = 0;

  StateHashTable<size> hashtable;

  uint32_t Ctxes[8] = {0, 1, 2, 3, 4, 5, 6, 7};
  StateMap<256> sm[4];
  
  StateHashTableLine* lines[8];
  State o1[0x10000];

  int counter = 0;
  int binary_idx = 0;
public:
  static constexpr int n_output = 4;

  ContextModel() {
    memset(o1, 0x0, sizeof(o1));
    searchHashTable();
  }

  void predict(uint8_t bit, Prob* pp) {
    
    for(int i=0;i<n_output;i++) {
      lines[i]->states[binary_idx].next(bit);
    }

    if(updateContext(bit))
      searchHashTable();

    for(int i=0;i<n_output;i++) {
      pp[i] = sm[i].predict(bit, lines[i]->states[binary_idx]);
    }
    
    /*
    o1[order1].next(bit);
    counter++;
    C0 = (C0 << 1) | bit;
    
    
    if(counter == 8) {
      Ctxes[0] = (uint32_t)C0 << 8;
      counter = 0;
      C0 = 1;
    }

    order1 = (Ctxes[0] + C0) & 0xFFFF;
    printf("%x, %x\n", order1, uint8_t(o1[order1]));
    *pp = sm[0].predict(bit, uint8_t(o1[order1]));

    if(counter == 0)
      binary_idx = 0;
    */
  }

  Context getContext(int n = 0){
    return Ctxes[n];
  }

private:

  void searchHashTable() {
    for(int i=0;i<n_output;i++) {
      lines[i] = hashtable[Ctxes[i]];
    }
  }

  bool updateContext(uint8_t bit) {
    counter++;

    if(bit)
      binary_idx = 2 * (binary_idx + 1);
    else
      binary_idx = 2 * binary_idx + 1;

    C0 = (C0 << 1) | bit;
    if(counter == 8) {
      C4 = (C4 << 8) | C0;

      Ctxes[0] = C0;
      Ctxes[1] = C4 & 0xFFFF;
      Ctxes[2] = C4 & 0xFFFFFF;
      Ctxes[3] = C4;

      counter = 0;
      C0 = 0;
      binary_idx = 0;
      return true;
    } else if(counter == 4){

      Ctxes[0] = (Ctxes[0] << 4) | C0;
      Ctxes[1] = (Ctxes[1] << 4) | C0;
      Ctxes[2] = (Ctxes[2] << 4) | C0;
      Ctxes[3] = (Ctxes[3] << 4) | C0;

      binary_idx = 0;
      return true;
    }

    return false;
  }

};

}