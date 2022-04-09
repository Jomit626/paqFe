#pragma once

#include <cstdint>
#include <cstdio>

// hash N bits data to M bits
template<int N, int M>
class TabHashing {
  static_assert(N > 0 && N <= 64);
  static_assert(M > 0 && M <= 32);
public:
  static constexpr int nByte = (N + 7) / 8;

  uint32_t X[nByte][256];
public:
  static constexpr uint32_t mask = (1 << M) - 1;

  uint32_t getX(int i, int j) const {
    return X[i][j];
  }

  void setX(int i, int j, uint32_t x) {
    X[i][j] = x & mask;
  }

  void setXUnsafe(int i, int j, uint32_t x) {
    X[i][j] = x;
  }

  uint32_t operator() (uint64_t val) const {
    uint32_t hashval = 0;
    for(int i=0;i<nByte;i++) {
      hashval ^= X[i][(val >> 8*i) & 0xFF];
    }
    return hashval & mask;
  }

  bool operator==(const TabHashing<N,M> &other) const {
    for(int i=0;i<nByte;i++) {
      for(int j=0;j<256;j++) {
      if(X[i][j] != other.X[i][j])
        return false;
      }
    }
    return true;
  }

  void dump_c(FILE* f, const char* name) {
    fprintf(f, "uint32_t %s[%d][256] = {\n", name, nByte);
    for(int i=0;i<nByte;i++) {
      fprintf(f, "{");
      for(int j=0;j<256;j++) {
        fprintf(f, "0X%08X,", X[i][j]);
        if(j % 8 == 7) {
          if(j == 255)
            fprintf(f, "},\n");
          else
            fprintf(f, "\n");
        }
      }

    }
    fprintf(f, "};\n");
  }

  void dump_scala(FILE* f, const char* name) {
    fprintf(f, "object %s {\n", name);
    fprintf(f, "  def apply() = {Seq(\n");
    for(int i=0;i<nByte;i++) {
      fprintf(f, "Seq(\n");
      for(int j=0;j<256;j++) {
        if(j % 8 == 7) {
          if(j == 255)
            fprintf(f, "0X%08X),\n", X[i][j]);
          else
            fprintf(f, "0X%08X,\n", X[i][j]);
        } else {
          fprintf(f, "0X%08X,", X[i][j]);
        }
      }
    }
    fprintf(f, "  )}\n");
    fprintf(f, "}\n");
  }
};

