#pragma once

#include <cstdio>
#include <cstdint>
#include <vector>

class Trace {
  size_t n;
  std::vector<uint64_t> o1t;
  std::vector<uint64_t> o2t;
  std::vector<uint64_t> o3t;
  std::vector<uint64_t> o4t;
public:
  Trace(const char* name) {
    FILE* f = fopen(name, "r");
    uint64_t CS[4];
    while(fscanf(f, "%lu,%lu,%lu,%lu\n", &CS[0], &CS[1], &CS[2], &CS[3]) == 4) {
      o1t.push_back(CS[0]);
      o2t.push_back(CS[1]);
      o3t.push_back(CS[2]);
      o4t.push_back(CS[3]);
    }
  }

  const std::vector<uint64_t>& getTrace(int i) const {
    if(i == 1)
      return o1t;
    else if (i == 2)
      return o2t;
    else if(i == 3)
      return o3t;
    else
      return o4t;
  }

};
