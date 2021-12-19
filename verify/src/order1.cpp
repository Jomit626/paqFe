#include <cstdio>
#include <cerrno>
#include <cstdlib>
#include <unistd.h>

#include <paqFe/paqFe.hpp>

using namespace paqFe::internal;

void generate_db(FILE* fin, FILE* fout) {
  Order1 model;

  uint8_t data[128];
  size_t n = 0;
  while((n = fread(data, 1, 128, fin))) {
    Prob p[8];
    for(int i=0;i<n;i++) {
      uint8_t byte = data[i];
      model.predict_byte(byte, p);
      for(int i=0;i<8;i++)
        fprintf(fout, "%d,%d\n", (byte >> (7 - i)) & 0x1, p[i]);
    }
  }
}