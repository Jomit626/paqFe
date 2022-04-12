#include <cstdio>
#include <cerrno>
#include <cstdlib>
#include <unistd.h>

#include <paqFe/models/StateMap.hpp>

using namespace paqFe::internal;

void generate_db(FILE* fin, FILE* fout) {
  for(int i=0;i<256;i++) {
    fprintf(fout, "%d\n",StaticStateMap::map[i]);
  }
}