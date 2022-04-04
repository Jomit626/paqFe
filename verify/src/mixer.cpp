#include <cstdio>

#include "paqFe/paqFe.hpp"
#include "paqFe/models/Group.hpp"
#include "paqFe/Mixer.hpp"

using namespace paqFe::internal;

FILE* gfout;

int mixer_id = 0;

template<int nFeature, int nHidden>
class MixerWrapped : Mixer<nFeature, nHidden> {
  using Parent = Mixer<nFeature, nHidden>;
  int id = -1;
  Prob Ps[nFeature];
public:

  MixerWrapped() : Parent() {
    id = mixer_id++;
  }

  void predict(const Prob* P, const Context *pctx, Prob *pp) {
    for(int i=0;i<nFeature;i++)
      Ps[i] = P[i];
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
    if(id != 1) // TODO: solve problem of forcing first output prob to 2048
      return;
    
    for(int i=0;i<nFeature;i++)
      fprintf(gfout, "%d,", Parent::X[i]);
    for(int i=0;i<nHidden;i++)
      fprintf(gfout, "%d,", Parent::prev_ctx[i]);
    fprintf(gfout, "%d,%d\n", bit, Parent::prev_prob);
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
