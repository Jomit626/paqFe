#include <cstdio>
#include <cerrno>
#include <cstdlib>
#include <unistd.h>
#include <chrono>
#include <omp.h>
#include <boost/random.hpp>
#include <vector>

#include "eval.hpp"

#define SEED 55555

using std::chrono::high_resolution_clock;

template<int N, int M>
void gen(const std::vector<uint64_t> &t, const char* name, FILE* c_file, FILE* scala_file) {
  constexpr size_t TableSize = 1 << M;
  constexpr int SearchCnt = 1000 * 32;

  Scores s_best;
  TabHashing<N, M> h_best;

  #pragma omp parallel
  {
    int nthread = omp_get_num_threads();
    int n = SearchCnt / nthread;
    bool first = true;

    Scores s, s_best_loc;
    TabHashing<N, M> h, h_best_loc;
    TableEntry *tab = new_table(TableSize);
    boost::random::taus88 rng(SEED + omp_get_thread_num());
    boost::random::uniform_int_distribution<> dist;

    for(int i=0;i<n;i++) {
      for(int j=0;j<h.nByte;j++) 
        for(int k=0;k<256;k++)
          h.setX(j, k, dist(rng));
      
      evalPE<N,M,TableSize>(h, t, tab, s);
      
      if(first || s_best_loc.weighted_hit_rate < s.weighted_hit_rate) {
        h_best_loc = h;
        s_best_loc = s;
        first = false;
      }

      if(i % 1000 == 0) {
        printf("current best:%lf\n", s_best_loc.weighted_hit_rate);
      }
    }

    #pragma omp critical 
    {
      if(s_best.weighted_hit_rate == 0 || s_best.weighted_hit_rate < s_best_loc.weighted_hit_rate) {
        h_best = h_best_loc;
        s_best = s_best_loc;
      }
    }

  }
  printf("result of %s:\nconfilict_rate:%lf\nhit_rate:%lf\nweighted_hit_rate:%lf\n",name, s_best.confilict_rate, s_best.hit_rate, s_best.weighted_hit_rate);
  h_best.dump_c(c_file, name);
  h_best.dump_scala(scala_file, name);
}

int main(int argc, char** argv) {
  int opt;
  const char *input_pathname = NULL, *output_pathname = NULL;
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

  FILE *c_file = fopen("../include/paqFe/models/TabHash.h", "w");
  FILE *scala_file = fopen("../include/paqFe/models/TabHash.scala", "w");
  fprintf(c_file, "//! This file is automatic generated. DO NOT MODIFY.\n#pragma once\n\n");
  fprintf(scala_file, "//! This file is automatic generated. DO NOT MODIFY.\n\n");

  Trace trace(input_pathname);
  printf("Input process done.\n");

  gen<21,16>(trace.getTrace(2), "O2HashTab", c_file, scala_file);
  gen<29,16>(trace.getTrace(3), "O3HashTab", c_file, scala_file);
  gen<37,17>(trace.getTrace(4), "O4HashTab", c_file, scala_file);
  gen<45,18>(trace.getTrace(5), "O5HashTab", c_file, scala_file);

  fclose(c_file);
  fclose(scala_file);
  return 0;
}

