#pragma once

#include <cstdint>
#include <cstdio>

// hash N bits data to M bits
template<int N, int M>
uint32_t tab_hashing(uint32_t tab[(N + 7)/8][256], uint64_t val) {
  constexpr uint32_t Mask = (1 << M) - 1;
  constexpr uint32_t nByte =  (N + 7) / 8;
  uint32_t hashval = 0;
  for(int i=0;i<nByte;i++) {
    hashval ^= tab[i][(val >> 8*i) & 0xFF];
  }
  return hashval & Mask;
}