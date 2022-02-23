#include <cstdio>
#include <cerrno>
#include <cstdlib>
#include <unistd.h>
#include <chrono>

#include "eval.hpp"

using std::chrono::high_resolution_clock;

int main(int argc, char** argv) {
  int opt;
  const char *input_pathname = NULL;
  while((opt = getopt(argc, argv, "i:")) != -1) {
    switch (opt) {
    case 'i':
      input_pathname = optarg;
      break;
    default:
      break;
    }
  }

  if(!input_pathname)
    return 1;

  Trace trace(input_pathname);
  printf("Input process done.\n");


  constexpr int N = 21;
  constexpr int M = 16;
  constexpr size_t TableSize = 1 << M;
  constexpr int SearchCnt = 1000;

  Scores s, s_best;
  HashFunc<N, M> h, h_best;
  TableEntry *tab = new_table(TableSize);

  for(int i=0;i<SearchCnt;i++) {
    for(int j=0;j<21;j++) {
      h.setX(j, rand());
    }
    evalPE<N,M,TableSize>(h, trace.getTrace(2), tab, s);
    if(s_best.weighted_hit_rate < s.weighted_hit_rate) {
      h_best = h;
      s_best = s;
    }
  }
  printf("good:%d,result:\nconfilict_rate:%lf\nhit_rate:%lf\nweighted_hit_rate:%lf\n",h.is_good(),s.confilict_rate, s.hit_rate, s.weighted_hit_rate);
  h.dump(stdout);
  return 0;
}