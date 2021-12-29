#pragma once

#include <cassert>
#include <cstring>
#include <algorithm>

#include "../types.hpp"
#include "CRC.hpp"
#include "StateMap.hpp"

namespace paqFe::internal {

using ::std::sort;

template<size_t HashSize, size_t HistorySize, size_t CRCWidth>
class MatchModel {
  static_assert(isPow2(HashSize), "HashSize of MatchModel must be a pow of 2");
  static_assert(isPow2(HistorySize), "HistorySize of MatchModel must be a pow of 2");

  static constexpr size_t HashMask = HashSize - 1;
  static constexpr size_t HistoryMask = HistorySize - 1;

  uint32_t hashtable[HashSize];

  uint32_t hidx = 0;
  uint8_t history[HistorySize];

  CRC<CRCWidth> crc;

  static constexpr int orders[] = {2, 4};
  static constexpr int ordreN = sizeof(orders) / sizeof(*orders) ;
  
  static constexpr int crc_buf_size = 5;
  uint32_t crc_buf[crc_buf_size] = {0xFFFFFFFF};

  struct Match {
    int valid = 0;
    uint32_t offset;
    uint32_t byte;

    bool operator<(const Match& other) {
      return (~valid && other.valid) || (offset < other.offset);
    }

  } matches[ordreN];


  // output status
  uint8_t byte = 0;
  int counter = 0;
  int use_match = 0;
  int prev_expect_bit;
public:

  static constexpr int OutputCnt = 1;

  MatchModel() {
    memset(history, 0x00, sizeof(history));
    memset(hashtable, 0x00, sizeof(hashtable));
    memset(matches, 0x00, sizeof(matches));
  }

  void init() {
    
  };

  void predict(uint8_t bit, Prob* pp) {
    uint32_t prob;

    byte = (byte << 1) | bit;
    counter ++;

    if(counter == 8) {
      counter = 0;
      use_match = 0; 
      
      predict_byte(byte);
    }

    if(matches[use_match].valid) {
      uint8_t expect_bit = (matches[use_match].byte >> (7 - counter)) & 0x1;
      if(expect_bit)
        prob = ProbEven + matches[use_match].offset;
      else
        prob = ProbEven - matches[use_match].offset;

      if(prev_expect_bit != bit) {
        // bit error
        matches[use_match].valid = 0;
        use_match = (use_match + 1) < ordreN ? (use_match + 1) : use_match;
      }
      prev_expect_bit = expect_bit;
    } else {
      prob = ProbEven;
    }
    *pp = prob;
  };
  
private:

  void calcute_hash(uint8_t byte) {
    uint32_t crc_pre = 0xFFFFFFFF;
    for(int i=0;i<crc_buf_size;i++) {
      uint32_t old_crc = crc_buf[i];

      crc_buf[i] = crc.next(crc_pre, byte);
      crc_pre = old_crc;
    }
  }

  uint32_t get_crc(int i) {
    return crc_buf[i];
  }

  uint32_t hashtable_replace(uint32_t hashval, uint32_t val) {
    uint32_t old = hashtable[hashval & HashMask];
    hashtable[hashval & HashMask] = val;
    return old;
  }

  size_t history_reverse_cmp(uint32_t i, uint32_t j) {
    uint32_t l = 0;
    uint8_t ci = history[i], cj = history[j];
    while(ci == cj && l < 8) {
      l++;

      i = (i - 1) & HistoryMask;
      j = (j - 1) & HistoryMask;
      ci = history[i], cj = history[j];
    }
    return l;
  }

  void predict_byte(uint8_t byte) {
    // recore byte
    history[hidx] = byte;
    uint32_t idx = hidx;
    hidx = (hidx + 1) & HistoryMask;
    
    calcute_hash(byte);

    for(int i=0;i<ordreN;i++) {
      int order = orders[i];
      uint32_t hashval = get_crc(order);

      // hashtable store pos of expect byte
      uint32_t val = hashtable_replace(hashval, hidx);
      matches[i].valid = 0;

      if(val != hidx) {
        int match_len = history_reverse_cmp((val - 1) & HistoryMask, idx);
        if(match_len > 0) {
          if(match_len > 65534)
            match_len = 65530;
          matches[i].valid = 1;
          matches[i].byte = history[val];
          uint32_t off = match_len * 200 ;
          matches[i].offset = off > 2047 ? 2047 : off;
        }
      }
    }

    sort(&matches[0], &matches[ordreN - 1]);
  }

};

}