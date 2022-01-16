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
  static constexpr int orderN = sizeof(orders) / sizeof(*orders) ;
  StateMap<1 << 16> sm[8];
  
  static constexpr int crc_buf_size = 5;
  uint32_t crc_buf[crc_buf_size] = {0xFFFFFFFF};
  uint32_t hashes[orderN] = {0};

  struct Match {
    int valid = 0;
    uint32_t len;
    uint32_t byte;
    uint32_t idx;

    bool operator<(const Match& other) {
      return (~valid && other.valid) || (len < other.len);
    }

  } matches[orderN];


  // output status
  uint8_t byte = 0;
  uint8_t prev_byte = 0;
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

  void predict(uint8_t bit, Prob *pp, Context *pctx) {
    sm[counter].update(bit);

    byte = (byte << 1) | bit;
    counter ++;

    update_match(bit, prev_expect_bit);

    if(counter == 8) {
      prev_byte = byte;
      counter = 0;
      
      find_match_byte(byte);
    }

    uint32_t ctx = (prev_byte << 8);
    if(matches[use_match].valid) {
      uint8_t expect_bit = (matches[use_match].byte >> (7 - counter)) & 0x1;
      ctx += (matches[use_match].len << 1) | expect_bit;

      prev_expect_bit = expect_bit;
    }

    *pctx = ctx;
    sm[counter].predict(ctx, pp);
  };

  void predict_byte(uint8_t byte, Prob *pp, Context *pctx, size_t stride = OutputCnt) {
    assert(counter == 0);

    for(int i=0;i<8;i++) {
      uint32_t ctx = (prev_byte << 8);
      uint8_t bit = (byte >> (7 - i)) & 0x1;

      if(matches[use_match].valid) {
        uint8_t expect_bit = (matches[use_match].byte >> (7 - i)) & 0x1;

        ctx += (matches[use_match].len << 1) | expect_bit;
        update_match(bit, expect_bit);
      }

      pctx[i * stride] = ctx;
      sm[i].predict(ctx, &pp[i * stride]);
      sm[i].update(bit);
    }
    find_match_byte(byte);
    prev_byte = byte;
  };

private:
  void update_match(uint8_t bit, uint8_t expect_bit) {
    if(matches[use_match].valid && expect_bit != bit) {
      matches[use_match].valid = 0;
      use_match = (use_match + 1) < orderN ? (use_match + 1) : use_match;
    }
  }

  void calcute_hash(uint8_t byte) {
    uint32_t crc_pre = 0xFFFFFFFF;
    for(int i=0;i<crc_buf_size;i++) {
      uint32_t old_crc = crc_buf[i];

      crc_buf[i] = crc.next(crc_pre, byte);
      crc_pre = old_crc;
    }
  }

  uint32_t get_hash(int i) {
    return crc_buf[orders[i]];
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

  void find_match_byte(uint8_t byte) {
    // recore byte
    history[hidx] = byte;
    uint32_t idx = hidx;
    hidx = (hidx + 1) & HistoryMask;
    
    calcute_hash(byte);

    if(matches[use_match].valid) {
      Match &m = matches[use_match];
      m.idx = (m.idx + 1) & HistoryMask;
      m.byte = history[m.idx];
      m.len = max(m.len, 1023);

      for(int i=use_match+1;i<orderN;i++) {
        matches[i].valid = 0;
      }


      // record hash
      for(int i=0;i<orderN;i++) {
        uint32_t hashval = get_hash(i);

        // hashtable store pos of expect byte
        uint32_t val = hashtable_replace(hashval, hidx);
      }
      return;
    }
    

    use_match = 0;
    for(int i=0;i<orderN;i++) {
      uint32_t hashval = get_hash(i);

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
          matches[i].idx = val;
          matches[i].len = match_len;
        }
      }
    }

    sort(&matches[0], &matches[orderN - 1]);
  }

};

}