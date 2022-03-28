#include <cstdio>

#include "paqFe/paqFe.hpp"
#include "paqFe/models/Group.hpp"
#include "paqFe/Mixer.hpp"

using namespace paqFe::internal;

FILE* gfout;

template<int nFeature, int nHidden>
class MixerWrapped : Mixer<nFeature, nHidden> {
  using Parent = Mixer<nFeature, nHidden>;
public:

  MixerWrapped() : Parent() {
  }

  void predict(const Prob* P, const Context *pctx, Prob *pp) {
    Parent::predict(P, pctx, pp);
  }

  void update(uint8_t bit) {
    Parent::update(bit);
    output(bit);
  }

  void update(uint8_t bit, Prob p) {
    Parent::update(bit, p);
    output(bit);
  }

private:
  void output(uint8_t bit) {
    for(int i=0;i<nHidden;i++) {
      Prob p = LUT.squash(Parent::X1[i]);
      fprintf(gfout, "%d,%d,%d\n", bit, p, Parent::lossCal(p , bit, Parent::Layer1LR));
    }
  }
};

using Model = paqFe::paqFeFile::Model;
using IPredictor = Predictor<8, Model, MixerWrapped<Model::nProb, Model::nCtx>>;

void generate_db(FILE* fin, FILE* fout) {
  gfout = fout;
  IPredictor &predictor = *(new IPredictor());

  uint8_t data[128];
  size_t n = 0;
  Prob prob = 2048;
  Prob p[8];
  while((n = fread(data, 1, 128, fin))) {
    for(int i=0;i<n;i++) {
      uint8_t byte = data[i];
      predictor.predict_byte(byte, &p[0]);
    }
  }

  delete &predictor;
}
