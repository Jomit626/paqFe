#include <cstdio>

#include "paqFe/paqFe.hpp"
#include "paqFe/models/Group.hpp"
#include "paqFe/Mixer.hpp"

using namespace paqFe::internal;

using Model = Orders<>;

void generate_db(FILE* fin, FILE* fout) {
  Model model;

  uint8_t data[128];
  size_t n = 0;
  while((n = fread(data, 1, 128, fin))) {
    Prob p[8][Model::nProb];
    Prob ctx[8][Model::nCtx];

    for(int i=0;i<n;i++) {
      uint8_t byte = data[i];
      model.predict_byte(byte, &p[0][0], &ctx[0][0], Model::nProb, Model::nCtx);

      for(int i=0;i<8;i++) {
        // bit
        fprintf(fout, "%d", (byte >> (7 - i)) & 0x1);
        // prob
        for(int j=0;j<Model::nProb;j++) {
          fprintf(fout, ",%d", p[i][j]);
        }
        for(int j=0;j<Model::nCtx;j++) {
          fprintf(fout, ",%d", ctx[i][j]);
        }
        fprintf(fout, "\n");
      }
    }
  }
}