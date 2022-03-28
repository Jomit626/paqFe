#include <cstdio>

#include "paqFe/paqFe.hpp"

using namespace paqFe::internal;

using Model = paqFe::paqFeFile::Model;

void generate_db(FILE* fin, FILE* fout) {
  fprintf(fout, 
    "nFeature: %d, nHidden: %d\n",
    Model::nProb, Model::nCtx
  );
}
