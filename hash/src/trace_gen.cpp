#include <cstdio>
#include <cerrno>
#include <cstdlib>
#include <unistd.h>

#include "paqFe/models/Orders.hpp"
using namespace paqFe::internal;

class OrdersCtx : Orders<12,16,16,17,17> {
  using Parent = Orders<12,16,16,17,17>;
public:
  void predict_byte(uint8_t byte, uint64_t *CS) {
    uint8_t nibble0 = byte >> 4;
    uint8_t nibble1 = byte & 0xF;

    CS[0] = C1;
    CS[1] = C2;
    CS[2] = C3;
    CS[3] = C4;
    CS[4] = C5;

    Parent::updateContextNibble0(nibble0);
    CS[5 + 0] = C1;
    CS[5 + 1] = C2;
    CS[5 + 2] = C3;
    CS[5 + 3] = C4;
    CS[5 + 4] = C5;

    Parent::updateContextNibble0(nibble1);
  }
};

void start(FILE* fin, FILE* fout) {
  OrdersCtx &model = *new OrdersCtx();
  uint64_t CS[10];
  uint8_t data[128];
  size_t n = 0;
  while((n = fread(data, 1, 128, fin))) {
    for(int i=0;i<n;i++) {
      uint8_t byte = data[i];
      model.predict_byte(byte, CS);

      fprintf(fout, "%lu,%lu,%lu,%lu,%lu\n%lu,%lu,%lu,%lu,%lu\n", CS[0], CS[1], CS[2], CS[3], CS[4], CS[5], CS[6], CS[7], CS[8], CS[9]);
    }
  }

  delete &model;
}

int main(int argc, char** argv) {
  int opt;
  const char *input_pathname = NULL, *output_pathname = NULL;
  while((opt = getopt(argc, argv, "i:o:")) != -1) {
    switch (opt) {
    case 'i':
      input_pathname = optarg;
      break;
    case 'o':
      output_pathname = optarg;
      break;
    default:
      break;
    }
  }

  if(!input_pathname || !output_pathname)
    return 1;

  FILE *fin = nullptr, *fout = nullptr;

  fin = fopen(input_pathname, "rb");
  fout = fopen(output_pathname, "w");

  if(!fin || !fout)
    return 1;

  start(fin, fout);

  return 0;
}