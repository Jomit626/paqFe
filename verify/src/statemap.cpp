#include <cstdio>

#include "paqFe/paqFe.hpp"
#include "paqFe/models/Group.hpp"
#include "paqFe/Mixer.hpp"

using namespace paqFe::internal;


void generate_db(FILE* fin, FILE* fout) {
  StateMap<1 << 8> sm;

  uint8_t data[128];
  size_t n = 0;
  while((n = fread(data, 1, 128, fin))) {

    Prob p;
    uint8_t C = 0;
    sm.predict(C, &p);
    for(int i=0;i<n;i++) {
      uint8_t byte = data[i];
      for(int j=7;j>=0;j--) {
        uint8_t bit = (byte >> j) & 0x1;
        sm.update(bit);
        fprintf(fout, "%d,%d,%d\n", C, bit, p);

        C = (C << 1) | bit;
        sm.predict(C, &p);
      }
    }
  }
}