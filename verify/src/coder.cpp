#include <cstdio>
#include <cerrno>
#include <cstdlib>
#include <unistd.h>

#include <paqFe/Coder.hpp>

using namespace paqFe::internal;

void generate_db(FILE* fin, FILE* fout) {
  Coder coder;

  uint8_t data[128];
  size_t n = 0;
  Prob prob = 2048;
  uint8_t output;
  while((n = fread(data, 1, 128, fin))) {
    for(int i=0;i<n;i++) {
      uint8_t byte = data[i];
      for(int j=0;j<8;j++) {
        if(coder.encode(((byte >> (7 - j))) & 0x1, prob, &output)) {
          fprintf(fout, "%d\n", output);
        }
        
        prob = prob + 1;
        if(prob > 4095)
          prob = 1;
      }
    }
  }

  // write tail
  uint32_t tail = coder.get_tail();
  fprintf(fout, "%d\n%d\n%d\n%d\n", (tail >> 24) & 0xFF, (tail >> 16) & 0xFF, (tail >> 8) & 0xFF, (tail >> 0) & 0xFF );
}