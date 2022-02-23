#pragma once

#include <cstdint>
#include <cstdio>

// hash N bits data to M bits
template<int N, int M>
class HashFunc {
  static_assert(N > 0 && N <= 64);
  static_assert(M > 0 && M <= 32);

  uint32_t X[N];
public:
  static constexpr uint32_t mask = (1 << M) - 1;
  uint32_t getX(int i) const {
    return X[i];
  }

  void setX(int i, uint32_t x) {
    X[i] = x & mask;
  }

  void setXUnsafe(int i, uint32_t x) {
    X[i] = x;
  }

  uint32_t operator() (uint64_t val) const {
    uint32_t hashval = 0;
    for(int i=0;i<N;i++) {
      if(val & (1 << i))
        hashval ^= X[i];
    }
    return hashval & mask;
  }

  bool operator==(const HashFunc<N,M> &other) const {
    for(int i=0;i<N;i++) {
      if(X[i] != other.X[i])
        return false;
    }
    return true;
  }

  void dump(FILE *f) {
    fprintf(f, "X[] = {\n");
    for(int i=0;i<N;i++) {
      if(i != N - 1)
        fprintf(f, "%x,\n", X[i]);
      else 
        fprintf(f, "%x}\n", X[i]);
    }
  }

  bool is_good() {  // are all bits used
    uint32_t x = X[0];
    for(int i=1;i<N;i++) {
      x |= X[i];
    }

    return x == mask;
  }
};
