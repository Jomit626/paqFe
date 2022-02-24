#pragma once

#include <cstring>

#include "hash.hpp"
#include "scores.hpp"
#include "trace.hpp"

struct TableEntry
{
  uint64_t val;
  uint64_t cnt;
};

template<int N, int M, size_t Size, typename Hash>
void evalPE(const Hash &h, const std::vector<uint64_t> &t, TableEntry* tab, Scores &s) {
  memset(tab, 0, sizeof(TableEntry) * Size);
  s.init();

  size_t n = t.size();
  for(int i=0;i<n;i++) {
    uint64_t val = t[i];
    uint32_t hashval = h(val);

    TableEntry &e = tab[hashval];
    if(e.val == val) {
      s.record_hit(e.cnt);

      e.cnt ++;
      
    } else {
      s.record_confilict(e.cnt);

      e.val = val;
      e.cnt = 1;
    }
  }

  s.concrate(n);
}

template<int N, int M, size_t Size, typename Hash>
void eval(const Hash &h, const std::vector<uint64_t> &t, Scores &s) {
  TableEntry* tab = new TableEntry[Size];

  evalPE<N, M, Size>(h, t, tab, s);

  delete tab;
}

TableEntry* new_table(size_t size) {
  return new TableEntry[size];
}

void delete_table(TableEntry* tab) {
  delete tab;
}
