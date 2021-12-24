#include <cstdio>
#include <cerrno>
#include <cstdlib>
#include <unistd.h>

#include <paqFe/paqFe.hpp>

using namespace paqFe;
using namespace paqFe::internal;

FILE* gfout;

class Order1Test : Order1 {
  FILE *f;
public:
  Order1Test(FILE* fout) : f(fout) {

  }
  void predict_byte(uint8_t byte, Prob *pp) {
    Order1::predict_byte(byte, pp);
    fprintf(f, "%03X\n%03X\n", (C1 >> 4) &0xFFF, C1 & 0xFFF);
  }
};

void generate_db(FILE* fin, FILE* fout) {
  Order1Test model(fout);

  uint8_t data[128];
  size_t n = 0;
  while((n = fread(data, 1, 128, fin))) {
    Prob p[8];
    for(int i=0;i<n;i++) {
      uint8_t byte = data[i];
      model.predict_byte(byte, p);
    }
  }
}